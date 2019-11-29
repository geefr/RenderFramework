#ifndef VECTOR_H
#define VECTOR_H

#include "common.h"

#include <vector>

namespace renderframework { namespace vector {

    class Vector
    {
    public:
        virtual ~Vector();

        /**
         * Query vertices
         *
         * @return vertex data for this vector
         */
        const std::vector<VertexDef>& vertices();
        const bool& usesIndices() const;
        const std::vector<GLuint>& indices();

        /**
         * Constructor
         *
         * Child classes must load whatever vector they want and populate
         * - mVertices
         *
         * These are optional, but if present size/structure must match mVertices
         * - mVertexColors
         * - mNormals
         *
         * Pretty please add all vertices in groups of 3, ordered anti-clockwise.
         * This framework draws triangles and nothing else (Right now via draw arrays so...)
         *
         * @brief Vector
         */
        Vector(bool useIndices);

        void addVertex(VertexDef vertex);
        void addIndex(GLuint index);

    protected:
        std::vector<VertexDef> mVertices;
        std::vector<GLuint> mIndices;
        bool mUsesIndices = false;
    };
} }

#endif // VECTOR_H
