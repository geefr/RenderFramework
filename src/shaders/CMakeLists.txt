
function( addShader SHADER )
    target_sources( rf PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/${SHADER} )

    #install( FILES ${CMAKE_CURRENT_SOURCE_DIR}/${SHADER} DESTINATION shaders )
endfunction()

addShader(vertex/default.vert)

addShader(tesselation/3vertpatch.tesscont)
addShader(tesselation/3vertpatch.tesseval)

addShader(fragment/ambient.frag )
addShader(fragment/diffuse.frag)
addShader(fragment/phong.frag)

install( DIRECTORY fragment
    DESTINATION shaders/ )
install( DIRECTORY tesselation
    DESTINATION shaders/ )
install( DIRECTORY vertex
    DESTINATION shaders/ )
