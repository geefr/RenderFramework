#ifndef VECTOR_OBJ_H
#define VECTOR_OBJ_H

#include "common.h"
#include "dataformats/vector/vector.h"

#include <string>

namespace renderframework { namespace vector {

    class Obj : public Vector
    {
    public:
        Obj( std::string objFile );
        virtual ~Obj();
    private:
        bool load();
        void objTexCoord(std::string line);
        void objVertNormal(std::string line);
        void objFace(std::string line);
        void objVert(std::string line);
        std::string mObjFile;
    };
} }

#endif
