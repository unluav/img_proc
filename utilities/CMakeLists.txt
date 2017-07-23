cmake_minimum_required(VERSION 2.8)
project( utilities )
option(CUDA_USE_STATIC_CUDA_RUNTIME OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package( OpenCV REQUIRED )
find_package( CUDA )

set(
	CUDA_NVCC_FLAGS
	${CUDA_NVCC_FLAGS};
    -O3 -gencode arch=compute_53,code=sm_53
)

include_directories("/usr/local/cuda-8.0/include")
include_directories(/usr/include/glib-2.0 /usr/include/libxml2 /usr/lib/aarch64-linux-gnu/glib-2.0/include/)

add_library( FRM STATIC FrameRateMonitor.cpp )
add_executable(video_test.exe video_test.cpp)
target_link_libraries(video_test.exe ${OpenCV_LIBS})
target_link_libraries(video_test.exe FRM)
