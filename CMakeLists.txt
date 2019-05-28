cmake_minimum_required(VERSION 3.14)
project(BPlusTree)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O2")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)  #指明最后可执行文件位置在bin文件夹
include_directories(${PROJECT_SOURCE_DIR}/include)  #指明头文件目录在include目录中

LINK_LIBRARIES(pthread) #链接标准库pthread

aux_source_directory(src/. SRC_LIST) #添加所有源文件
add_executable(BPlusTree ${SRC_LIST} include/test.h src/test.cpp) #编译所用到的文件
#add_executable(BPlusTree src/main.cpp include/node.h include/nodeTest.h src/nodeTest.cpp include/BPlusTree.h include/BPlusTreeTest.h src/BPlusTreeTest.cpp include/dataGenerate.h src/dataGenerate.cpp)


# cmake使用方法
# cd build
# cmake ../
# make
# cd ../bin
# ./xxx(xxx is excutable file)