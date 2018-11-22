#include "phongmaterialbare.h"
#include "engine/shaderprogram.h"

namespace renderframework { namespace materials {
    PhongMaterialBare::PhongMaterialBare() {}
    PhongMaterialBare::PhongMaterialBare( vec3 ambient, vec3 diffuse, vec3 specular, float shininess )
        : mAmbient(ambient)
        , mDiffuse(diffuse)
        , mSpecular(specular)
        , mShininess(shininess)
    {}

    PhongMaterialBare::~PhongMaterialBare() {}

    void PhongMaterialBare::setUniforms( ShaderProgram& shader )
    {
        glUniform3fv(shader.uniform("material.ambient"), 1, value_ptr(mAmbient));
        glUniform3fv(shader.uniform("material.diffuse"), 1, value_ptr(mDiffuse));
        glUniform3fv(shader.uniform("material.specular"), 1, value_ptr(mSpecular));
        glUniform1f(shader.uniform("material.shininess"), mShininess);
    }

    void PhongMaterialBare::registerUniforms( ShaderProgram& shader )
    {
        shader.regUniform("material.ambient");
        shader.regUniform("material.diffuse");
        shader.regUniform("material.specular");
        shader.regUniform("material.shininess");
    }
} }
