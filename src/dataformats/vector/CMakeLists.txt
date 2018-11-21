
add_library( rfvector SHARED
 vector.h
 vector.cpp )

if( VECTOR_PRIMITIVES )
    add_subdirectory(primitives)
endif()
