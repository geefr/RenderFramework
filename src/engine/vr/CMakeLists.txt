
set( rfvr_cpp
  env.cpp
  device.cpp
  controller.cpp
)

set( rfvr_h
    env.h
    device.h
    controller.h
)

include_directories( ${OPENVR_ROOT}/include )
link_directories( ${OPENVR_ROOT}/lib )

add_library( rfvr ${LIBTYPE} ${rfvr_cpp} ${rfvr_h} )

if( MSVC )
  target_link_libraries( rfvr ${OPENVR_LIB} glm)
endif()

install( TARGETS rfvr
         ARCHIVE DESTINATION lib/RenderFramework
         LIBRARY DESTINATION lib/RenderFramework )
install( FILES ${rfvr_h} DESTINATION include/RenderFramework/engine/vr )

