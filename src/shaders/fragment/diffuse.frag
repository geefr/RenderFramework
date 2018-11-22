#version 450 core

struct Light
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 position;
};
uniform Light light;

in vec3 worldPos_frag_in;
in vec2 texCoord_frag_in;
in vec4 texColor_frag_in;
in vec3 worldNormal_frag_in;

layout(location = 0) out vec4 fragColor;

void main(void)
{
    vec3 normal = normalize(worldNormal_frag_in);
    vec3 lightDir = normalize(light.position - worldPos_frag_in);

    vec3 diffuse = light.diffuse * max(dot(normal, lightDir), 0.0);



    fragColor = (vec4(light.ambient, 1.0) + vec4(diffuse, 1.0)) * texColor_frag_in;
}
