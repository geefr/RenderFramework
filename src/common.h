#ifndef COMMON_H
#define COMMON_H

#include <GL/glew.h>
#include <GL/wglew.h>
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
