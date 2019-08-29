

include_directories( ${OPENVR_ROOT}/include )
link_directories( ${OPENVR_ROOT}/lib )

add_executable( test_openvr main.cpp )

target_link_libraries(test_openvr
  rf
  rfvr
  rfvector
  rfvector_primitives
  rfraster_png
  OpenGL::GL
  glfw
  ${OPENVR_LIB}
  )

#target_link_libraries(test_openvr
#  ${OPENVR_LIB}
#  rf
#  rfraster
#  rfraster_png
#  ${PNG_LIBRARY}
#  rfvector
#  rfvector_primitives
#  OpenGL::GL glm glfw ${GLEW_LIBRARIES}
#)

install( TARGETS test_openvr
         ARCHIVE DESTINATION bin )


