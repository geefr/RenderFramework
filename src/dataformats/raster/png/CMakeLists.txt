
find_package( PNG REQUIRED )

add_library( rfraster_png pngloader.h pngloader.cpp )
target_link_libraries( rfraster_png PNG::PNG )
