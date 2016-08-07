cmake_minimum_required (VERSION 2.8)
project (circleDetection)
find_package (OpenCV REQUIRED)
add_executable (CircleDetector circleDetector.cpp)
target_link_libraries (CircleDetector ${OpenCV_LIBS})

