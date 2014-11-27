cmake_minimum_required(VERSION 2.8)
project("OpenCV tools")

find_package( OpenCV REQUIRED )
#add_definitions( -std=c++11 )

add_subdirectory(diff_gauss)
add_subdirectory(gaussian)
add_subdirectory(range_stretch)
add_subdirectory(threshold)