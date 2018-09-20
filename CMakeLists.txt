CMAKE_MINIMUM_REQUIRED(VERSION 3.0)
PROJECT(cv_tutorial)
FIND_PACKAGE(OpenCV 	REQUIRED )
include_directories ("${OpenCV_INCLUDE_DIRS}")
#Required for uco thinstation. May not be required in your own pc
#SET(CMAKE_CXX_FLAGS " -D_GLIBCXX_USE_CXX11_ABI=0 -m32 -g")
LINK_LIBRARIES(${OpenCV_LIBS} )
add_executable(practica1 main.cpp functions.cpp)
