third_party_library=/home/yipeng/thirdlib
g++ -g -O3 -W -Wall video_transport_client.cpp -o client -I $third_party_library/glog/include/ -I $third_party_library/gflags/include/ \
-I $third_party_library/opencv/include/ -L $third_party_library/glog/lib/ -L $third_party_library/gflags/lib -L $third_party_library/opencv/lib \
-lglog -lgflags -lopencv_highgui -lopencv_imgcodecs -lopencv_core -lopencv_imgproc -lopencv_videoio -lopencv_objdetect -lpthread -std=gnu++11
echo -e "\033[1;31mbash脚本开始执行... \033[0m";
read -p "请输入媒体文件名:" file_name;

./client -flagfile=client_flagfile_configure -media_path=$file_name
echo -e "\033[1;32mbash脚本执行完毕... \033[0m";
