#encoding:utf-8
#python scons script file

current_dir = '/home/yipeng/workspace/c++/socket_videocapture/'  #项目目录
thirdlib_dir = '/home/yipeng/thirdlib/'                          #第三方库目录

glog_inc = thirdlib_dir + 'glog/include'
gflags_inc = thirdlib_dir + 'gflags/include'
ffmpeg_inc = thirdlib_dir + 'ffmpeg/include'
protobuf_inc = thirdlib_dir + 'protobuf/include'
opencv_inc = thirdlib_dir + 'opencv/include'

glog_lib = thirdlib_dir + 'glog/lib'
gflags_lib = thirdlib_dir + 'gflags/lib'
ffmpeg_lib = thirdlib_dir + 'ffmpeg/lib'
protobuf_lib = thirdlib_dir + 'protobuf/lib'
opencv_lib = thirdlib_dir + 'opencv/lib'

#cpp头文件路径
include_dirs = [
    glog_inc, 
    gflags_inc, 
    ffmpeg_inc, 
    protobuf_inc, 
    opencv_inc, 
]

#cpp库文件路径
lib_dirs = [
    glog_lib, 
    gflags_lib, 
    ffmpeg_lib, 
    protobuf_lib, 
    opencv_lib, 
]

#cpp库文件  动态链接库 或者静态库
libs = [
    'glog', 
    'gflags',
    'protobuf', 
    'opencv_core', 
    'opencv_highgui', 
    'opencv_imgproc', 
    'opencv_imgcodecs', 
    'opencv_videoio', 
    'opencv_objdetect', 
    'avcodec', 
    'avformat', 
    'avutil', 
    'swscale', 
]

#链接时的标志  -Wl指定运行可执行程序时 去哪个路径找动态链接库
link_flags = [
    '-pthread', 
    '-fsanitize=address', 
    '-Wl,-rpath=' + ":".join(lib_dirs), 
    #'-Wl,-rpath-link=' + ":".join(lib_dirs), 
]

#cpp的编译标志
cpp_flags = [
    '-O3',                     #更好的编译优化
    '-fsanitize=address',      #asan的选项 编译链接都要用
    '-fno-omit-frame-pointer', #堆栈跟踪  
    '-g', 
    '-std=gnu++11', 
    '-W',                      #显示所有warning
    '-Wall', 
    '-D local_camera',         #宏定义
]

server_source = [
    current_dir + 'video_transport_server.cpp', 
]

client_source = [
    current_dir + 'video_transport_client.cpp', 
]

#program生成可执行文件
video_decode = Program(
    target = 'server',            #可执行文件名   -o
    source = server_source,       #源文件列表
    CPPPATH = include_dirs,       #头文件路径列表 -I
    LIBPATH = lib_dirs,           #库文件路径列表 -L
    LIBS = libs,                  #库文件  -l
    LINKFLAGS = link_flags,       #链接的标志  -
    CPPFLAGS = cpp_flags,         #编译的标志  -
)

#安装
bin_path = current_dir + 'bin'
Install(bin_path, "server")

#program生成可执行文件
video_decode = Program(
    target = 'client',            #可执行文件名   -o
    source = client_source,       #源文件列表
    CPPPATH = include_dirs,       #头文件路径列表 -I
    LIBPATH = lib_dirs,           #库文件路径列表 -L
    LIBS = libs,                  #库文件  -l
    LINKFLAGS = link_flags,       #链接的标志  -LINK_FLAGS
    CPPFLAGS = cpp_flags,         #编译的标志  -CXX_FLAGS
)

#安装
bin_path = current_dir + 'bin'
Install(bin_path, "client")

#SharedLibrary生成动态链接库
#libvideo_decode = SharedLibrary(
#    target = 'video_decode', 
#    source = src_list, 
#    CPPPATH = include_dirs, 
#    LIBPATH = lib_dirs, 
#    LIBS = libs, 
#    LINKFLAGS = link_flags, 
#    CPPFLAGS = cpp_flags, 
#)


#shared_path = current_dir + 'lib'
#Install(shared_path, libvideo_decode)
