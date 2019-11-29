#ifndef VECTOR_OBJ_H
#define VECTOR_OBJ_H

#include "common.h"
#include "dataformats/vector/vector.h"

#include <string>
#include <array>

namespace renderframework { namespace vector {

    class Obj : public Vector
    {
    public:
        Obj( std::string objFile );
        virtual ~Obj();
    private:
        struct FaceIndex {
          GLuint vert;
          GLuint tex;
          GLuint norm;
        };
        using Face = std::array<FaceIndex,3>;

        bool load();
        void objTexCoord(std::string line);
        void objVertNormal(std::string line);
        void objFace(std::string line);
        void objVert(std::string line);
        std::string mObjFile;

        std::vector<glm::vec3> mVertCoords;
        std::vector<glm::vec2> mVertTexCoords;
        std::vector<glm::vec3> mVertNorms;
        std::vector<glm::vec<3,GLuint>> mObjIndices;
    };
} }

#endif
