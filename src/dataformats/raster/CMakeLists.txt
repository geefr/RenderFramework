
add_library( rfraster SHARED
 raster.h
 raster.cpp )

if( RASTER_PNG )
  add_subdirectory(png)
endif()

