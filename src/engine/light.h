#ifndef LIGHT_H
#define LIGHT_H

#include "common.h"

namespace renderframework
{
    class ShaderProgram;
    class Light
    {
    public:
        Light();
        Light(vec3 ambient, vec3 diffuse, vec3 specular, vec3 position);
        static void registerUniforms( ShaderProgram& shader );
        void setUniforms( ShaderProgram& shader );

        vec3 mAmbient = {0.f,0.f,0.f};
        vec3 mDiffuse = {0.f,0.f,0.f};
        vec3 mSpecular = {0.f,0.f,0.f};
        vec3 mPosition = {0.f,0.f,0.f};
    };
}

#endif // LIGHT_H
