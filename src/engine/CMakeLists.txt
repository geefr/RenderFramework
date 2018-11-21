
add_library( rf 
 engine.cpp
 engine.h
 shaderprogram.cpp
 shaderprogram.h
)

target_link_libraries( rf OpenGL::GL )
