project( "Paint Tools" )

add_library( paint_tools paint_tools.cpp )
target_link_libraries( paint_tools ${OpenCV_LIBS})
add_library( OpenCV_tools OpenCV_tools.cpp)