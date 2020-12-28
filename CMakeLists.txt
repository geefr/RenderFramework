cmake_minimum_required(VERSION 3.6)

# Project Settings
set(RF_VERSION_MAJOR 2)
set(RF_VERSION_MINOR 0)
set(RF_VERSION_PATCH 0)
set(RF_VERSION_STRING ${RF_VERSION_MAJOR}.${RF_VERSION_MINOR}.${RF_VERSION_PATCH})
set(RF_VERSION_ABI 2)

project(RenderFramework VERSION ${RF_VERSION_STRING})

if( NOT CMAKE_BUILD_TYPE AND NOT MSVC )
  set( CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE )
  set_property( CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Release" "Debug" "MinSizeRel" "RelWithDebInfo" )
endif()

if( NOT LIBTYPE )
    set( LIBTYPE SHARED CACHE STRING "The type of libraries to build" FORCE )
    set_property( CACHE LIBTYPE PROPERTY STRINGS "SHARED" "STATIC" )
endif()


option( ENABLE_TEST "Enable tests" ON )
option( RASTER_PNG "PNG Support via libPNG" ON )
option( VECTOR_PRIMITIVES "Basic primitive shapes" ON )
option( VECTOR_OBJ "Wavefront .obj support" ON )
option( ENABLE_VR "Enable VR support" OFF )

# Compiler/Tool requirements
set( CMAKE_CXX_STANDARD 17 )
set(OpenGL_GL_PREFERENCE GLVND)

if( NOT MSVC )
  set( CMAKE_CXX_FLAGS "-Werror=all" )
endif()

# Package/System requirements
find_package( glfw3 REQUIRED )
find_package( OpenGL REQUIRED )
find_package( glm REQUIRED )
if( MSVC )
  find_package( glew REQUIRED )
  if( MSVC )
    include_directories( ${GLEW_INCLUDE_DIRS} )
  endif()
endif()
find_package( Boost REQUIRED )

include_directories(${PROJECT_SOURCE_DIR}/src)

if( ENABLE_VR )
  if( MSVC )
    # TODO: Paths hardcoded for now
    if( NOT OPENVR_ROOT )
      set( OPENVR_ROOT "" CACHE PATH "Root directory of OpenVR SDK" FORCE )
      message( FATAL_ERROR "VR Support enabled but OPENVR_ROOT not set")
    endif()

    set( OPENVR_LIB "openvr_api64" )
  else()
    set( OPENVR_LIB "openvr_api" )
  endif()
endif()


# The meat of the project
add_subdirectory(src)

if( ENABLE_TEST )
    add_subdirectory(test)
endif()
