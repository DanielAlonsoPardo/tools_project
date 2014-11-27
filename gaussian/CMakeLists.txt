project( "Gaussian blur" )

add_executable( gauss main.cpp )
target_link_libraries( gauss ${OpenCV_LIBS})

