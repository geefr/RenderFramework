#version 450 core
uniform sampler2D texture0;
uniform bool enableTexture0;

struct Ambient
{
    float intensity;
    vec3 color;
};
struct Light
{
    Ambient ambient;
};
uniform Light light;

//uniform float light_ambient_intensity;

in vec3 worldPos_frag_in;
in vec2 texCoord_frag_in;
in vec4 texColor_frag_in;

layout(location = 0) out vec4 fragColor;

void main(void)
{
    if( enableTexture0 )
    {
      fragColor = texture2D(texture0, texCoord_frag_in) * texColor_frag_in;
    }
    else
    {
      fragColor = texColor_frag_in;
    }

    vec3 ambient = light.ambient.intensity * light.ambient.color;

    fragColor = vec4(ambient, 1.0) * fragColor;
}
