
function( addShader SHADER )
    target_sources( rf PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/${SHADER} )
endfunction()

addShader(vertex/default.vert)

addShader(tesselation/3vertpatch.tesscont)
addShader(tesselation/3vertpatch.tesseval)

addShader(fragment/ambient.frag)
addShader(fragment/diffuse.frag)
addShader(fragment/phong.frag)
