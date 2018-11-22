#ifndef PHONGMATERIALBARE_H
#define PHONGMATERIALBARE_H

#include "common.h"
#include "material.h"

namespace renderframework { namespace materials {
    class PhongMaterialBare : public Material
    {
    public:
        PhongMaterialBare();
        PhongMaterialBare( vec3 ambient, vec3 diffuse, vec3 specular, float shininess );
        ~PhongMaterialBare() final override;

        void setUniforms( ShaderProgram& shader ) final override;
        static void registerUniforms( ShaderProgram& shader );

        // Reflectance values for each component
        vec3 mAmbient = {0.f,0.f,0.f};
        vec3 mDiffuse = {0.f,0.f,0.f};
        vec3 mSpecular = {0.f,0.f,0.f};
        float mShininess = 0.f;
    };
} }

#endif // PHONGMATERIALBARE_H
