cmake_minimum_required(VERSION 2.8)
project("OpenCV tools")

find_package( OpenCV REQUIRED )
add_definitions( -std=c++11 )

if (NOT DEFINED ENV{TFG})
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
else ()
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY $ENV{TFG}/bin)
endif()

file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

include_directories(OpenCV_utilities)

#programs
add_subdirectory(diff_gauss)
add_subdirectory(gaussian)
add_subdirectory(range_stretch)
add_subdirectory(threshold)
add_subdirectory(deperspectivize)
add_subdirectory(blacknwhite)
add_subdirectory(chromakey)
add_subdirectory(crop)
add_subdirectory(average)
#libs
add_subdirectory(OpenCV_utilities)
#misc
add_subdirectory(test)

