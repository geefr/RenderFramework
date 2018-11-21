
find_package( PNG REQUIRED )

add_library( pngloader pngloader.h pngloader.cpp )
target_link_libraries( pngloader PNG::PNG )
