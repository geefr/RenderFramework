#ifndef ENGINE_H
#define ENGINE_H

#include <iostream>
#include <exception>
#include <stdexcept>
#include <fstream>
#include <vector>

#include "dataformats/raster/png/rfpng.h"
#include "dataformats/vector/vector.h"
#include "dataformats/vector/primitives/cube.h"
#include "common.h"
#include "shaderprogram.h"
#include "materials/phongmaterialbare.h"
#include "nodes/meshnodeda.h"
#include "light.h"

namespace renderframework
{
    class Engine
    {
    public:
        Engine();
        void init();
        void init2();
        void loop( float width, float height );

        [[noreturn]] static void quit(std::string msg);

        vec4 viewExtent{ -1.0f, 1.0f, -1.0f, 1.0f };

        vec2 viewCenter{ 0.0f, 0.0f };
        vec2 cursorPos{ 0.0f, 0.0f };
        vec2 windowSize{ 800.0f, 600.0f };
        vec3 viewRot{ 0.0f, 0.0f, 0.0f };


        std::map<std::string, std::shared_ptr<materials::PhongMaterialBare>> mMaterials;
        std::map<std::string, std::shared_ptr<ShaderProgram>> mShaders;
        std::shared_ptr<nodes::Node> mNode;
        Light light;

    private:
        void checkGlError();
        GLuint loadTexture( std::string fileName );
        void setVertexAttribPointers(GLint vertCoordAttrib, GLint texCoordAttrib, GLint vertColorAttrib);
        void printMat4x4(mat4x4 mat, std::string name);

        GLuint vao;
        GLuint catTexture;
    };
}

#endif // ENGINE_H
