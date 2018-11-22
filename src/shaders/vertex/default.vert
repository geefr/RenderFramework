#version 450 core
uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelMatrix;

in vec3 vertCoord;
in vec2 vertTexCoord;
in vec4 vertTexColor;
in vec3 vertNormal;

out vec3 worldPos_tessCont_in;
out vec2 texCoord_tessCont_in;
out vec4 texColor_tessCont_in;
out vec3 worldNormal_tessCont_in;

void main(void)
{
    // model -> world
    worldPos_tessCont_in = vec3(modelMatrix * vec4(vertCoord, 1.0));
    texCoord_tessCont_in = vertTexCoord;
    texColor_tessCont_in = vertTexColor;

    // Lighting calculations are performed in world space
    // This uses the 'normal matrix' which scales/rotates correctly for the normals
    worldNormal_tessCont_in = mat3(transpose(inverse(modelMatrix))) * vertNormal;
}
