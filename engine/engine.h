#ifndef ENGINE_H
#define ENGINE_H

#include "linmath.h"

#include <iostream>
#include <exception>
#include <stdexcept>
#include <fstream>
#include <vector>

#include "dataformats/raster/png/pngloader.h"
#include "dataformats/vector/gdal/gdalloader.h"
#include "common.h"
#include "featuremodel/featuremodel.h"
#include "engine/shaderprogram.h"

class Engine
{
public:
    Engine();
void init();
void loop( int width, int height );

[[noreturn]] static void quit(std::string msg);


vec4 viewExtent{ -180.0f, 180.0f, -90.0f, 90.0f };

vec2 viewCenter{ 0.0f, 0.0f };
vec2 cursorPos{ 0.0f, 0.0f };
vec2 windowSize{ 800.0f, 600.0f };
vec3 viewRot{ 0.0f, 0.0f, 0.0f };
vec3 viewRotDelta{ 0.0f, 0.0f, 0.0f };

private:

void checkGlError();

GLuint loadAndCompileShader( GLenum shaderType, std::string path );
GLuint linkShaders( const std::initializer_list<GLuint>& v );
GLuint loadTexture( std::string fileName );
GLuint createVertexBuffer( const std::vector<VertexDef>& data );
void setVertexAttribPointers(GLint vertCoordAttrib, GLint texCoordAttrib, GLint vertColorAttrib);
void printMat4x4(mat4x4 mat, std::string name);




    struct Light
    {
        struct Ambient
        {
            vec3 color;
            float intensity;
        };
        struct Diffuse
        {

        };

        Ambient ambient;
        Diffuse diffuse;
    };
    Light light;

    ShaderProgram texturedVectorShader;

    GLuint vao;

    GLuint catTexture;
    GLuint vertexBuffer;
        std::vector<VertexDef> vertexData;
};

#endif // ENGINE_H
