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
        const std::vector<vec3>& vertices();
    protected:
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
        Vector();

        void addVertex(vec3 vertex);
        void addVertex(vec3 vertex, vec4 color);
        void addVertex(vec3 vertex, vec4 color, vec3 normal);

        std::vector<vec3> mVertices;
        std::vector<vec4> mVertexColors;
        std::vector<vec3> mNormals;
    };
} }

#endif // VECTOR_H
