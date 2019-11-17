#项目名称 
project(ASHENQAQ)
#cmake的最低版本
cmake_minimum_required(VERSION 3.5.1)

#添加选项 支持c++11
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -g -O3 -W -Wall -Dlocal_camera")
#add_definitions(`pkg-config --cflags --libs opencv`)

#Release mode
set(CMAKE_BUILD_TYPE Release) 
set(third_party_library /home/yipeng/thirdlib)
set(client_source video_transport_client.cpp)
set(server_source video_transport_server.cpp)
set(link_library glog gflags pthread)

#自动查找构建工程所需程序库 找到后会预定义几个变量
find_package(OpenCV REQUIRED)

#头文件的路径 这里用了相对路径
include_directories(${OpenCV_INCLUDE_DIRS}
                    ${third_party_library}/glog/include
                    ${third_party_library}/gflags/include)

#库文件的路径
link_directories(${OpenCV_LIBRARY_DIRS}
                 #${third_party_library}/opencv/lib
                 ${third_party_library}/glog/lib
                 ${third_party_library}/gflags/lib)

#可执行文件的名称
add_executable(server ${server_source})
target_link_libraries(server ${link_library} ${OpenCV_LIBS})

add_executable(client ${client_source})
target_link_libraries(client ${link_library} ${OpenCV_LIBS})





	

