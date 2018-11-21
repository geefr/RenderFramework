cmake_minimum_required(VERSION 3.6)

# Project Settings
set(RF_VERSION_MAJOR 1)
set(RF_VERSION_MINOR 0)
set(RF_VERSION_PATCH 1)
set(RF_VERSION_STRING ${RF_VERSION_MAJOR}.${RF_VERSION_MINOR}.${RF_VERSION_PATCH})
set(RF_VERSION_ABI 1)

project(RenderFramework VERSION ${RF_VERSION_STRING})

if( NOT CMAKE_BUILD_TYPE )
  set( CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE )
  set_property( CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Release" "Debug" "MinSizeRel" "RelWithDebInfo" )
endif()

option( ENABLE_TEST "Enable tests" ON )
option( ENABLE_THIRDPARTY "Enable third party components" ON )
option( RASTER_PNG "PNG Support via libPNG" ON )
option( VECTOR_PRIMITIVES "Basic primitive shapes" ON )

# Compiler/Tool requirements
set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_FLAGS "-Werror=all" )

# Package/System requirements
find_package( glfw3 REQUIRED )
find_package( OpenGL REQUIRED )
find_package( glm REQUIRED )

# Top level includes and such
if( ENABLE_THIRDPARTY )
    # linmath is needed by pretty much everything
    include_directories( ${PROJECT_SOURCE_DIR}/thirdparty/linmath )
endif()

include_directories(${PROJECT_SOURCE_DIR}/src)

# The meat of the project
add_subdirectory(src)

if( ENABLE_TEST )
    add_subdirectory(test)
endif()

if( ENABLE_THIRDPARTY )
    # Nothing here that needs linking yet
    #add_subdirectory(thirdparty)
endif()
