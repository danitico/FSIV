cmake_minimum_required (VERSION 3.0)
project (videoseg)
set(CMAKE_CXX_FLAGS "-std=c++11")
find_package(OpenCV 3.2 REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/tclap-1.2.1/include ${OpenCV_INCLUDE_DIRS})
LINK_LIBRARIES(${OpenCV_LIBS})

add_executable(vsegbase vsegbase.cpp)

