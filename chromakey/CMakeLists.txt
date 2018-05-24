project( "Chromakey" )

add_executable( chromakey main.cpp )
target_link_libraries( chromakey ${OpenCV_LIBS} OpenCV_tools)

