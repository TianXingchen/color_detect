cmake_minimum_required(VERSION 3.10)
project(color_detect)

set(CMAKE_CXX_STANDARD 11)
include_directories(/home/Downloads/openCV/lib/include)
include_directories(/home/Downloads/openCV/lib/opencv)
include_directories(/home/Downloads/openCV/lib/opencv2)
set(SOURCE_FILES main.cpp)

find_package(OpenCV REQUIRED)

add_executable(color_detect ${SOURCE_FILES})

target_link_libraries(color_detect ${OpenCV_LIBS})