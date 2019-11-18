#docker build c/c++ program
#基础镜像
FROM ubuntu:latest

#维护者信息
MAINTAINER yipeng7 <986300260@qq.com>

#构建镜像时运行的命令
RUN mkdir /usr/src/client_videocapture
#工作目录
WORKDIR /usr/src/client_videocapture

RUN mkdir thirdlib 
RUN mkdir thirdlib/opencv
RUN mkdir thirdlib/opencv/include
RUN mkdir thirdlib/opencv/lib
RUN mkdir thirdlib/glog
RUN mkdir thirdlib/glog/include
RUN mkdir thirdlib/glog/lib
RUN mkdir thirdlib/gflags
RUN mkdir thirdlib/gflags/include
RUN mkdir thirdlib/gflags/lib

#和COPY一样 不过后面是压缩文件会自动解压 是url会下载资源(wget)
ADD client_videocapture.tar ./
ADD opencv_include.tar ./thirdlib/opencv/include
ADD opencv_lib.tar ./thirdlib/opencv/lib
ADD glog_include.tar ./thirdlib/glog/include
ADD glog_lib.tar ./thirdlib/glog/lib
ADD gflags_include.tar ./thirdlib/gflags/include
ADD gflags_lib.tar ./thirdlib/gflags/lib

#安装依赖包
ADD .bashrc /root
ADD sources.list /etc/apt
RUN apt-get update && apt-get install gcc && \
apt-get install 

#设置环境变量
ENV third_party_library=./thirdlib LD_LIBRARY_PATH=./thirdlib/opencv/lib:./thirdlib/glog/lib:./thirdlib/gflags/lib:$LD_LIBRARY_PATH

#运行容器时 容器默认运行的应用
CMD ["bash client_build_with_camera.sh"]

