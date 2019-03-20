
find_package( PNG REQUIRED )

add_library( rfraster_png ${LIBTYPE} rfpng.h rfpng.cpp )
target_link_libraries( rfraster_png PNG::PNG )

install( TARGETS rfraster_png
         ARCHIVE DESTINATION lib/RenderFramework
         LIBRARY DESTINATION lib/RenderFramework )
install( FILES rfpng.h DESTINATION include/RenderFramework/dataformats/raster/png )
