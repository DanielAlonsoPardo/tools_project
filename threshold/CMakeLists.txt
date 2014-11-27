project( "Threshold" )

add_executable( thresh main.cpp )
target_link_libraries( thresh ${OpenCV_LIBS})
