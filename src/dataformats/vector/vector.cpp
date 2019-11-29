#include "vector.h"

namespace renderframework { namespace vector {
    Vector::Vector(bool useIndices)
      : mUsesIndices(useIndices)
    {
    }

    Vector::~Vector()
    {
    }

    const std::vector<VertexDef>& Vector::vertices() { return mVertices; }
    const std::vector<GLuint>& Vector::indices() { return mIndices; }
    const bool& Vector::usesIndices() const { return mUsesIndices; }
    void Vector::addVertex(VertexDef vertex) { mVertices.emplace_back(vertex); }
    void Vector::addIndex(GLuint index) { mIndices.emplace_back(index); }
} }
