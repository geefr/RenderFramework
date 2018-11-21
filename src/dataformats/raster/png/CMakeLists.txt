
find_package( PNG REQUIRED )

add_library( rfraster_png rfpng.h rfpng.cpp )
target_link_libraries( rfraster_png PNG::PNG )
