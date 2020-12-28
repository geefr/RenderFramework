
add_library( rfvector_primitives ${LIBTYPE} cube.h cube.cpp )
target_link_libraries( rfvector_primitives glfw )
if( MSVC )
  target_link_libraries( rfvector_primitives glm )
endif()
install( TARGETS rfvector_primitives
         ARCHIVE DESTINATION lib/RenderFramework
         LIBRARY DESTINATION lib/RenderFramework )
install( FILES cube.h DESTINATION include/RenderFramework/dataformats/vector/primitives )
