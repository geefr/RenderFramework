
add_executable( test_glfw-cube main.cpp cubescene.cpp objscene.cpp )

target_link_libraries( test_glfw-cube
    rf
    rfraster
    rfraster_png
    rfvector
    rfvector_primitives
    rfvector_obj
    glfw )

install( TARGETS test_glfw-cube
         ARCHIVE DESTINATION bin )
