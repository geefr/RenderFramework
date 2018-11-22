#include "light.h"
#include "shaderprogram.h"

namespace renderframework
{
    Light::Light() {}
    Light::Light(vec3 ambient, vec3 diffuse, vec3 specular, vec3 position)
        : mAmbient(ambient)
        , mDiffuse(diffuse)
        , mSpecular(specular)
        , mPosition(position)
    {}

    void Light::registerUniforms( ShaderProgram& shader )
    {
        shader.regUniform("light.ambient");
        shader.regUniform("light.diffuse");
        shader.regUniform("light.specular");
        shader.regUniform("light.position");
    }

    void Light::setUniforms( ShaderProgram& shader )
    {
        glUniform3fv(shader.uniform("light.ambient"), 1, value_ptr(mAmbient));
        glUniform3fv(shader.uniform("light.diffuse"), 1, value_ptr(mDiffuse));
        glUniform3fv(shader.uniform("light.specular"), 1, value_ptr(mSpecular));
        glUniform3fv(shader.uniform("light.position"), 1, value_ptr(mPosition));
    }
}
