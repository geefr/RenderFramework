
add_executable( test_glfw-multiscene main.cpp )

target_link_libraries( test_glfw-multiscene
    rf
    rfraster
    rfraster_png
    rfvector
    rfvector_primitives
    glfw )

install( TARGETS test_glfw-multiscene
         ARCHIVE DESTINATION bin )
