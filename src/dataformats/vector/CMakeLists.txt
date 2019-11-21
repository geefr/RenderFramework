
add_library( rfvector ${LIBTYPE} vector.h vector.cpp )
target_link_libraries( rfvector glm )
if( VECTOR_PRIMITIVES )
    add_subdirectory(primitives)
endif()
if( VECTOR_OBJ )
  add_subdirectory(obj)
endif()

install( TARGETS rfvector
         ARCHIVE DESTINATION lib/RenderFramework
         LIBRARY DESTINATION lib/RenderFramework )
install( FILES vector.h DESTINATION include/RenderFramework/dataformats/vector )
