third_party_library=/home/yipeng/thirdlib
g++ -g -O3 -W -Wall -D local_camera video_transport_client.cpp -o client -I $third_party_library/glog/include/ -I $third_party_library/gflags/include/ \
-I $third_party_library/opencv/include/ -L $third_party_library/glog/lib/ -L $third_party_library/gflags/lib -L $third_party_library/opencv/lib \
-lglog -lgflags -lopencv_highgui -lopencv_imgcodecs -lopencv_core -lopencv_imgproc -lopencv_videoio -lopencv_objdetect -lpthread -std=gnu++11
./client -flagfile=client_flagfile_configure
