cmake_minimum_required(VERSION 3.6)

project(RenderFramework)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/thirdparty/linmath )
add_executable(${PROJECT_NAME}
    main.cpp
    engine/shaderprogram.h engine/shaderprogram.cpp
    engine/engine.h engine/engine.cpp
    )
target_link_libraries(${PROJECT_NAME} pngloader glfw OpenGL::GL)

add_subdirectory(dataformats)
