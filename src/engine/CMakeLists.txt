
add_library( rf 
 engine.cpp
 engine.h
 shaderprogram.cpp
 shaderprogram.h

 materials/material.h
 materials/material.cpp
 materials/phongmaterialbare.h
 materials/phongmaterialbare.cpp
)

target_link_libraries( rf rfraster OpenGL::GL )
