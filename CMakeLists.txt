cmake_minimum_required(VERSION 2.8)
project( Undistort )

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( Undistort main.cpp )
target_sources(Undistort PRIVATE ocam_functions.cpp)
target_link_libraries( Undistort ${OpenCV_LIBS} )