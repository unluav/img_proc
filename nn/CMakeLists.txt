cmake_minimum_required ( VERSION 2.8 )
project ( neural_net )

option ( CUDA_USE_STATIC_CUDA_RUNTIME OFF )
find_package ( OpenCV REQUIRED )
find_package ( CUDA REQUIRED )

set ( CMAKE_CXX_STANDARD_REQUIRED ON )
set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11" )
set ( CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS}; -O3 -gencode arch=compute_61,code=sm_61 )

include_directories ( "/usr/local/cuda-8.0/include" )
include_directories ( "/usr/include/glib-2.0" "/usr/include/libxml2" "/usr/lib/aarch64-linux-gnu/glib-2.0/include/" )

cuda_add_executable (
        main.exe
        main.cpp
)

target_link_libraries ( main.exe ${OpenCV_LIBS} )
