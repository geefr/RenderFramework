#version 450 core

struct Light
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 position;
};

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform Light light;
uniform Material material;

// Eye position in world space
uniform vec3 eyePos;

in vec3 worldPos_frag_in;
in vec2 texCoord_frag_in;
in vec4 texColor_frag_in;
in vec3 worldNormal_frag_in;

layout(location = 0) out vec4 fragColor;

void main(void)
{
    vec3 normal = normalize(worldNormal_frag_in);
    vec3 lightDir = normalize(light.position - worldPos_frag_in);
    vec3 eyeDir = normalize(eyePos - worldPos_frag_in);

    // Ambient component
    vec4 ambient = vec4(light.ambient * material.ambient, 1.0);

    // Diffuse component
    vec4 diffuse = vec4(light.diffuse * max(dot(normal, lightDir) * material.diffuse, 0.0), 1.0) ;

    // Specular component
    vec3 specularReflectDir = reflect(-lightDir, normal);
    vec4 specular = vec4(light.specular * pow(max(dot(eyeDir, specularReflectDir), 0.0), material.shininess) * material.specular, 1.0);

    fragColor = (ambient + diffuse + specular);// * texColor_frag_in;
}
