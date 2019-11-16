g++ -g -O3 -W -Wall video_transport_client.cpp -o client -I /home/yipeng/thirdlib/glog/include/ -I /home/yipeng/thirdlib/gflags/include/ -I /home/yipeng/thirdlib/opencv/include/ -L /home/yipeng/thirdlib/glog/lib/ -L /home/yipeng/thirdlib/gflags/lib -L /home/yipeng/thirdlib/opencv/lib -lglog -lgflags -lopencv_highgui -lopencv_imgcodecs -lopencv_core -lopencv_imgproc -lopencv_videoio -lopencv_objdetect -lpthread -std=gnu++11
echo -e "\033[1;31mbash脚本开始执行... \033[0m";
read -p "请输入媒体文件名:" file_name;
media_path=/home/yipeng/videos/$file_name
echo $media_path

./client -flagfile=client_flagfile_configure -media_path=$media_path
echo -e "\033[1;32mbash脚本执行完毕... \033[0m";
