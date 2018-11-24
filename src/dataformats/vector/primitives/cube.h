#ifndef STATICSHAPELOADER_H
#define STATICSHAPELOADER_H

#include "common.h"
#include "dataformats/vector/vector.h"

namespace renderframework { namespace vector {

    class Cube : public Vector
    {
    public:
        Cube( vec3 center, vec3 dimensions );
        virtual ~Cube();

        vec3 mCenter;
    };
} }

#endif // STATICSHAPELOADER_H
