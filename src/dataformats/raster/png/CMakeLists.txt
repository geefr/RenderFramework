
find_package( PNG REQUIRED )

if( MSVC )
  include_directories( ${PNG_PNG_INCLUDE_DIR} )
endif()

add_library( rfraster_png ${LIBTYPE} rfpng.h rfpng.cpp )
target_link_libraries( rfraster_png rfraster )
if( MSVC )
  target_link_libraries( rfraster_png ${PNG_LIBRARY} )
else()
  target_link_libraries( rfraster_png PNG::PNG )
endif()

install( TARGETS rfraster_png
         ARCHIVE DESTINATION lib/RenderFramework
         LIBRARY DESTINATION lib/RenderFramework )
install( FILES rfpng.h DESTINATION include/RenderFramework/dataformats/raster/png )
