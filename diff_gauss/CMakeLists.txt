project( "Difference of gaussians" )

add_executable( diff_gauss main.cpp )
target_link_libraries( diff_gauss ${OpenCV_LIBS})
