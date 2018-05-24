project( "Average" )

add_executable( average main.cpp )
target_link_libraries( average ${OpenCV_LIBS} )

