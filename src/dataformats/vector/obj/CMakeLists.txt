
add_library( rfvector_obj ${LIBTYPE} obj.h obj.cpp )
target_link_libraries( rfvector_obj glm )
install( TARGETS rfvector_obj
         ARCHIVE DESTINATION lib/RenderFramework
         LIBRARY DESTINATION lib/RenderFramework )
install( FILES obj.h DESTINATION include/RenderFramework/dataformats/vector/obj )
