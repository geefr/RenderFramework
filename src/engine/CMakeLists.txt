
add_library( rf 
 engine.cpp
 engine.h
 shaderprogram.cpp
 shaderprogram.h

 light.h
 light.cpp

 nodes/node.h
 nodes/node.cpp
 nodes/meshnodeda.h
 nodes/meshnodeda.cpp

 materials/material.h
 materials/material.cpp
 materials/phongmaterialbare.h
 materials/phongmaterialbare.cpp
)

target_link_libraries( rf rfraster OpenGL::GL )
