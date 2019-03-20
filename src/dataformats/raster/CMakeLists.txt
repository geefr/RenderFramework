
add_library( rfraster ${LIBTYPE} raster.cpp raster.h )

if( RASTER_PNG )
  add_subdirectory(png)
endif()

install( TARGETS rfraster
         ARCHIVE DESTINATION lib/RenderFramework
         LIBRARY DESTINATION lib/RenderFramework )
install( FILES raster.h DESTINATION include/RenderFramework/dataformats/raster )
