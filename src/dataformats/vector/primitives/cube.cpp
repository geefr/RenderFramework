#include "cube.h"

namespace renderframework { namespace vector {

    Cube::Cube( vec3 center, vec3 dimensions )
    {
        // Add as 1x1x1 around 0,0,0 then scale

        // Bottom
        addVertex({-.5f,-.5f,.5f});
        addVertex({ .5f,-.5f,-.5f});
        addVertex({ .5f,-.5f,-.5f});

        addVertex({ .5f,-.5f,-.5f});
        addVertex({ .5f,-.5f,.5f});
        addVertex({-.5f,-.5f,-.5f});

        // Top
        // Left
        // Right
        // Front
        addVertex({-.5f,-.5f, .5f});
        addVertex({ .5f,-.5f, .5f});
        addVertex({ .5f, .5f, .5f});

        addVertex({-.5f, .5f, .5f});
        addVertex({ .5f, -.5f, .5f});
        addVertex({ .5f, .5f, .5f});

        // Back
    }

    Cube::~Cube()
    {
    }
} }
