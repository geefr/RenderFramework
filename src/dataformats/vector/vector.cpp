#include "vector.h"

namespace renderframework { namespace vector {
    Vector::Vector()
    {
    }

    Vector::~Vector()
    {
    }

    const std::vector<vec3>& Vector::vertices() { return mVertices; }
    const std::vector<vec4>& Vector::vertexColors() { return mVertexColors; }
    const std::vector<vec3>& Vector::normals() { return mNormals; }
    void Vector::addVertex(vec3 vertex) { mVertices.emplace_back(vertex); }
    void Vector::addVertex(vec3 vertex, vec4 color) { mVertexColors.emplace_back(color); addVertex(vertex); }
    void Vector::addVertex(vec3 vertex, vec4 color, vec3 normal) { mNormals.emplace_back(normal); addVertex(vertex, color); }
} }
