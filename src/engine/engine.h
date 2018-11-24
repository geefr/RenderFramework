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
        void loop( float width, float height );

        [[noreturn]] static void quit(std::string msg);


        vec4 viewExtent{ -1.0f, 1.0f, -1.0f, 1.0f };

        vec2 viewCenter{ 0.0f, 0.0f };
        vec2 cursorPos{ 0.0f, 0.0f };
        vec2 windowSize{ 800.0f, 600.0f };
        vec3 viewRot{ 0.0f, 0.0f, 0.0f };
        vec3 viewRotDelta{ 0.01f, 0.015f, 0.02f };
        //vec3 viewRotDelta{ 0.f,0.f,0.f };


    private:

        void checkGlError();

        //GLuint loadAndCompileShader( GLenum shaderType, std::string path );
        //GLuint linkShaders( const std::initializer_list<GLuint>& v );
        GLuint loadTexture( std::string fileName );
        //GLuint createVertexBuffer( const std::vector<VertexDef>& data );
        void setVertexAttribPointers(GLint vertCoordAttrib, GLint texCoordAttrib, GLint vertColorAttrib);
        void printMat4x4(mat4x4 mat, std::string name);

        Light light;
        GLuint vao;
        GLuint catTexture;

        std::map<std::string, std::shared_ptr<materials::PhongMaterialBare>> mMaterials;
        std::map<std::string, std::shared_ptr<ShaderProgram>> mShaders;

        std::shared_ptr<nodes::Node> mNode;
    };
}

#endif // ENGINE_H
