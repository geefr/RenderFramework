
set( rf_cpp engine.cpp
    shaderprogram.cpp
    light.cpp
    nodes/node.cpp
    nodes/meshnodeda.cpp
    materials/material.cpp
    materials/phongmaterialbare.cpp
)

set( rf_h engine.cpp
    engine.h
    shaderprogram.h
    light.h
    nodes/node.h
    nodes/meshnodeda.h
    materials/material.h
    materials/phongmaterialbare.h
)

add_library( rf ${LIBTYPE} ${rf_cpp} ${rf_h} )

target_link_libraries( rf rfraster OpenGL::GL )

install( TARGETS rf
         ARCHIVE DESTINATION lib/RenderFramework
         LIBRARY DESTINATION lib/RenderFramework )
install( FILES ${rf_h} DESTINATION include/RenderFramework/ )
