#ifndef COMMON_H
#define COMMON_H

#ifdef _WIN32
# include <GL/glew.h>
# include <GL/wglew.h>
#else
# define GL_GLEXT_PROTOTYPES
#endif

#include <GL/gl.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

namespace renderframework
{

    struct VertexDef
    {
        vec3 coord;
        vec2 texCoord;
        vec4 color;
        vec3 normal;
    };
}

#endif // COMMON_H
