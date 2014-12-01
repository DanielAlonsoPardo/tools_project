project( "Deperspectivize" )

add_executable( deperspectivize main.cpp )
target_link_libraries( deperspectivize ${OpenCV_LIBS} paint_tools)

