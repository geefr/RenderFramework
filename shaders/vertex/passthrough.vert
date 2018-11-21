#version 450 core
uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelMatrix;

in vec3 vertCoord;
in vec2 vertTexCoord;
in vec4 vertTexColor;

out vec3 worldPos_tessCont_in;
out vec2 texCoord_tessCont_in;
out vec4 texColor_tessCont_in;

void main(void)
{
    worldPos_tessCont_in = (modelMatrix * vec4(vertCoord, 1.0)).xyz;
    texCoord_tessCont_in = vertTexCoord;
    texColor_tessCont_in = vertTexColor;
}
