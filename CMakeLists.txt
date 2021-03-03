cmake_minimum_required(VERSION 2.8)
project( undistort )

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( undistort main.cpp )
target_sources(undistort PRIVATE ocam_functions.cpp)
target_link_libraries( undistort ${OpenCV_LIBS} )