
add_library( rfvector ${LIBTYPE} vector.h vector.cpp )

if( VECTOR_PRIMITIVES )
    add_subdirectory(primitives)
endif()

install( TARGETS rfvector
         ARCHIVE DESTINATION lib/RenderFramework
         LIBRARY DESTINATION lib/RenderFramework )
install( FILES vector.h DESTINATION include/RenderFramework/dataformats/vector )
