project( "Test" )

add_executable( test main.cpp )
target_link_libraries( test ${OpenCV_LIBS} paint_tools )

