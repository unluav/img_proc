cmake_minimum_required ( VERSION 2.8 )
project ( track_centers )

option ( CUDA_USE_STATIC_CUDA_RUNTIME OFF )
find_package ( OpenCV REQUIRED )
find_package ( CUDA )
add_library ( FRM STATIC ../utilities/FrameRateMonitor.cpp )

set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11" )
set ( CMAKE_CXX_STANDARD_REQUIRED ON )
set ( CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS}; -O3 -gencode arch=compute_53,code=sm_53 )

include_directories ( "/usr/local/cuda-8.0/include" )
include_directories ( "/usr/include/glib-2.0" "/usr/include/libxml2" "/usr/lib/aarch64-linux-gnu/glib-2.0/include/" )

add_executable (
	tc_test.exe
	tc_test.cpp
	track_centers.cpp
)

target_link_libraries ( tc_test.exe ${OpenCV_LIBS} )
