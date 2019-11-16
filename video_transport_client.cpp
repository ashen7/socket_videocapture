/*
客户端：
    1、创建一个socket，用函数socket()； 
　　2、设置socket属性，用函数setsockopt();* 可选 
　　3、绑定IP地址、端口等信息到socket上，用函数bind();* 可选 
　　4、设置要连接的对方的IP地址和端口等属性； 
　　5、连接服务器，用函数connect()； 
　　6、收发数据，用函数send()和recv()，或者read()和write(); 
　　7、关闭网络连接；
*/
//IPv4资源有限 所以IP指的都是一个局域网的IP 
//MAC地址真正定位一台计算机 然后通过target_port端口定位一台计算机的某个进程
#ifdef __linux__
#include <cstdio>				//perror在这里声明
#include <assert.h>				//断言
#include <errno.h>				//error
#include <cstring>				//之前的string.h 一些字符串操作 strerror在这里声明

#include <unistd.h>				//这几个全是Linux socket的头文件
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <iostream>
#include <string>				//string类模板的头文件

#include <gflags/gflags.h>		//google的glags和glog头文件
#include <glog/logging.h>
#include <opencv2/opencv.hpp>   //opencv库

#define IMG_WIDTH 640			//传输帧的宽
#define IMG_HEIGHT 480			//传输帧的高
#define BUFFER_SIZE IMG_WIDTH * IMG_HEIGHT * 3 / 32  //640*480*3/32 一张图片分32次包来发送
#endif

using cv::VideoCapture;

DEFINE_int32(target_port, 8888, "what is the localhost target_port?");
DEFINE_string(target_ip, "10.209.149.204", "what is the localhost target_ip?");
DEFINE_string(media_path, "", "video capture pathname");

struct SendBuffer {
	char buffer[BUFFER_SIZE];
	int is_frame_end;
} send_frame_data;

static void InitializeGlog(int argc, char* argv[]);                  //初始化glog日志
static int RunTcpClient(int32_t target_port,const char* target_ip);  //socket客户端
static int SendFrame(int client_socket, const cv::Mat& frame);       //发送帧给服务端

static inline bool ValidatePort(const char* filename,int32_t value) {       //google-gflags参数的注册函数
	if (value > 0 && value < 32768) {
		return true;
	} else {
		return false;
	}
}

int main(int argc,char* argv[]) {
    InitializeGlog(argc, argv);
    LOG(INFO) << "Start Google Logging!";
    
	bool is_valid_port = google::RegisterFlagValidator(&FLAGS_target_port,&ValidatePort);
	LOG(INFO) << "断言gflags回调函数是否成功...";
	assert(is_valid_port);  //断言
	LOG(INFO) << "gflags注册回调函数成功！";
	
    //Run Client
    int32_t target_port = FLAGS_target_port;
    const char* target_ip = FLAGS_target_ip.c_str();
	RunTcpClient(target_port, target_ip);
	if (!errno)
		LOG(INFO) << "run programming is successful!"; 
	else
		LOG(ERROR) << "run programming is fail, reason: " << strerror(errno);

    LOG(INFO) << "Close Google Logging!";
    google::ShutdownGoogleLogging();
	return 0;
}

void InitializeGlog(int argc, char *argv[]) {
    //初始化gflags 
    google::ParseCommandLineFlags(&argc, &argv, true);
    //初始化glog
    google::InitGoogleLogging(argv[0]);
    //设置日志输出目录 以及最低级别
    google::SetLogDestination(google::GLOG_INFO, "log/");
    //大于指定级别的日志输出到标准输出
    google::SetStderrLogging(google::GLOG_INFO);

    //设置stderr的颜色消息
    FLAGS_colorlogtostderr = true;
    //设置日志前缀是否应该添加到每行输出
    FLAGS_log_prefix = true;
    //设置可以缓冲日志的最大秒数 0表示实时输出日志
    FLAGS_logbufsecs = 0;
    //设置最大日志文件大小 MB
    FLAGS_max_log_size = 10;
}

int RunTcpClient(int32_t target_port,const char* target_ip) {
	//1.创建套接字 得到网络连接的文件描述符
	int client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	//2.连接到目标主机
	struct sockaddr_in client_addr;
	//bzero(&(server_addr.sin_zero),8);  //字节用0填充
	//void *memset(void *s,int c,size_t n) 将已开辟内存空间 s 的首 n 个字节的值设为值 c。
	memset(&(client_addr), 0, sizeof(client_addr));
	client_addr.sin_family = AF_INET;                   //使用IPv4地址
	client_addr.sin_addr.s_addr = inet_addr(target_ip); //具体的target_ip地址
	client_addr.sin_port = htons(target_port);          //端口
    connect(client_socket, (struct sockaddr*)&client_addr, sizeof(struct sockaddr));

    int is_local_camera = -1;
#ifdef local_camera
	//打开默认摄像头
    cv::VideoCapture video_capture(0);
    is_local_camera = 1;
#else
	//打开本地媒体文件
    cv::VideoCapture video_capture(FLAGS_media_path.c_str());
    is_local_camera = 0;
#endif
	if (!video_capture.isOpened()) {
		LOG(WARNING) << "打开摄像头失败!" << strerror(errno);
        return -1;
	}
    
    cv::Mat frame;
	//3.客户端发送消息给服务端
    //先发一条信息 是本地摄像头的帧数据还是媒体文件的帧数据
    send(client_socket, (char*)&is_local_camera, sizeof(int), 0);
	while (true) {
        //cap.read()读给frame 
		video_capture >> frame;     
        cv::resize(frame, frame, cv::Size(640, 480));
		//给客户端发送每一帧的图像
		if (0 != SendFrame(client_socket, frame)) {
            LOG(WARNING) << "send frame to server is fail...";
            break;
        }
	}

	//4.关闭套接字
	close(client_socket);
    return 0;
}

int SendFrame(int client_socket, const cv::Mat& frame) {
	//每次发送一个包(15行数据) 一张图片发送32次包
	for (int packet_number = 0; packet_number < 32; packet_number++) {
		int current_packet_number = IMG_HEIGHT / 32 * packet_number;
        //一个包里一行的行大小是640 * 3个字节
		for (int current_line = 0; current_line < IMG_HEIGHT / 32; current_line++) {
            //记录当前已经赋值了多少行数据 
			int current_line_data = current_line * IMG_WIDTH * 3;
            //拿到每行帧数据的指针
		    const uchar* current_line_frame_data = frame.ptr<uchar>(current_line + current_packet_number);
            //一行的每个像素点
			for (int current_pixel = 0; current_pixel < IMG_WIDTH * 3; current_pixel++) {
				send_frame_data.buffer[current_line_data + current_pixel] = current_line_frame_data[current_pixel];
			}
		}

		if (packet_number == 31)
			send_frame_data.is_frame_end = 2;
		else
			send_frame_data.is_frame_end = 1;
		//这样发送的消息就是15*640*3个字节的 视频一帧32分之一的数据了
		send(client_socket, (char*)(&send_frame_data), sizeof(send_frame_data), 0);
	}

    return 0;
}
