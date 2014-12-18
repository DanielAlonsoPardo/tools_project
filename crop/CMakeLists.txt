project( "Crop" )

add_executable( crop main.cpp )
target_link_libraries( crop ${OpenCV_LIBS} paint_tools )

