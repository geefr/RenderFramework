
if( ENABLE_VR )
  add_subdirectory(vr)
endif()

set( rf_cpp 
    engine.cpp
    scene.cpp
    defaultframebuffer.cpp
    framebufferrendertotexture.cpp
    shaderprogram.cpp
    light.cpp
    nodes/node.cpp
    nodes/meshnodeda.cpp
    nodes/meshnodedi.cpp
    materials/material.cpp
    materials/phongmaterialbare.cpp
    colliders/collider.cpp
    colliders/cubecollider.cpp
)

set( rf_h
    engine.h
    scene.h
    framebuffer.h
    defaultframebuffer.h
    framebufferrendertotexture.h
    shaderprogram.h
    light.h
    )
set( rf_nodes_h
    nodes/node.h
    nodes/meshnodeda.h
    nodes/meshnodedi.h
    )
set( rf_mat_h
    materials/material.h
    materials/phongmaterialbare.h
)
set( rf_col_h
    colliders/collider.h
    colliders/cubecollider.h
)

add_library( rf ${LIBTYPE} ${rf_cpp} ${rf_h} ${rf_nodes_h} ${rf_mat_h} ${rf_col_h} )

target_link_libraries( rf rfraster OpenGL::GL glm glfw)
if( MSVC )
  target_link_libraries( rf ${GLEW_LIBRARIES} )
endif()

install( TARGETS rf
         ARCHIVE DESTINATION lib/RenderFramework
         LIBRARY DESTINATION lib/RenderFramework )
install( FILES ${rf_h} DESTINATION include/RenderFramework/engine/ )
install( FILES ${rf_nodes_h} DESTINATION include/RenderFramework/engine/nodes/ )
install( FILES ${rf_mat_h} DESTINATION include/RenderFramework/engine/materials/ )
install( FILES ${rf_col_h} DESTINATION include/RenderFramework/engine/colliders/ )
