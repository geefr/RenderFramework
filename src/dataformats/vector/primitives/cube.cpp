#include "cube.h"

namespace renderframework { namespace vector {

    Cube::Cube( vec3 center, vec3 dimensions )
        : mCenter(center)
    {
        // Add as 1x1x1 around 0,0,0 then scale

        // Bottom/
        addVertex({ -.5f,-.5f,-.5f }, { 1.f,0.f,0.f,1.f }, { 0.f,-1.f,0.f });
        addVertex({  .5f,-.5f,-.5f }, { 1.f,0.f,0.f,1.f }, { 0.f,-1.f,0.f });
        addVertex({ -.5f,-.5f,.5f }, { 1.f,0.f,0.f,1.f }, { 0.f,-1.f,0.f });

        addVertex({-.5f,-.5f,.5f}, {1.f,0.f,0.f,1.f}, {0.f,-1.f,0.f});
        addVertex({ .5f,-.5f,-.5f}, {0.f,1.f,0.f,1.f}, {0.f,-1.f,0.f});
        addVertex({ .5f,-.5f, .5f}, {0.f,0.f,1.f,1.f}, {0.f,-1.f,0.f});

        // Top/
        addVertex({-.5f,.5f, .5f}, {1.f,0.f,0.f,1.f}, {0.f,1.f,0.f});
        addVertex({ .5f,.5f, .5f}, {0.f,1.f,0.f,1.f}, {0.f,1.f,0.f});
        addVertex({-.5f,.5f,-.5f}, {0.f,0.f,1.f,1.f}, {0.f,1.f,0.f});

        addVertex({-.5f,.5f,-.5f}, {1.f,0.f,0.f,1.f}, {0.f,1.f,0.f});
        addVertex({ .5f,.5f, .5f}, {0.f,1.f,0.f,1.f}, {0.f,1.f,0.f});
        addVertex({ .5f,.5f,-.5f}, {0.f,0.f,1.f,1.f}, {0.f,1.f,0.f});

        // Left/
        addVertex({-.5f,.5f,-.5f}, {1.f,0.f,0.f,1.f}, {-1.f,0.f,0.f});
        addVertex({-.5f,-.5f,-.5f}, {0.f,1.f,0.f,1.f}, {-1.f,0.f,0.f});
        addVertex({-.5f,.5f,.5f}, {0.f,0.f,1.f,1.f}, {-1.f,0.f,0.f});

        addVertex({-.5f,.5f,.5f}, {1.f,0.f,0.f,1.f}, {-1.f,0.f,0.f});
        addVertex({-.5f,-.5f,-.5f}, {0.f,1.f,0.f,1.f}, {-1.f,0.f,0.f});
        addVertex({-.5f,-.5f,.5f}, {0.f,0.f,1.f,1.f}, {-1.f,0.f,0.f});

        // Right
        addVertex({.5f, .5f, .5f}, {1.f,0.f,0.f,1.f}, {1.f,0.f,0.f});
        addVertex({.5f,-.5f, .5f}, {0.f,1.f,0.f,1.f}, {1.f,0.f,0.f});
        addVertex({.5f,-.5f,-.5f}, {0.f,0.f,1.f,1.f}, {1.f,0.f,0.f});

        addVertex({.5f, .5f, .5f}, {1.f,0.f,0.f,1.f}, {1.f,0.f,0.f});
        addVertex({.5f,-.5f,-.5f}, {0.f,1.f,0.f,1.f}, {1.f,0.f,0.f});
        addVertex({.5f,.5f,-.5f}, {0.f,0.f,1.f,1.f}, {1.f,0.f,0.f});

        // Front/
        addVertex({-.5f,-.5f, .5f}, {1.f,0.f,0.f,1.f}, {0.f,0.f,1.f});
        addVertex({ .5f, .5f, .5f}, {0.f,1.f,0.f,1.f}, {0.f,0.f,1.f});
        addVertex({-.5f, .5f, .5f}, {0.f,0.f,1.f,1.f}, {0.f,0.f,1.f});

        addVertex({-.5f,-.5f, .5f}, {1.f,0.f,0.f,1.f}, {0.f,0.f,1.f});
        addVertex({ .5f,-.5f, .5f}, {0.f,1.f,0.f,1.f}, {0.f,0.f,1.f});
        addVertex({ .5f, .5f, .5f}, {0.f,0.f,1.f,1.f}, {0.f,0.f,1.f});

        // Back/
        addVertex({-.5f,-.5f,-.5f}, {1.f,0.f,0.f,1.f}, {0.f,0.f,-1.f});
        addVertex({-.5f, .5f,-.5f}, {0.f,1.f,0.f,1.f}, {0.f,0.f,-1.f});
        addVertex({ .5f,-.5f,-.5f}, {0.f,0.f,1.f,1.f}, {0.f,0.f,-1.f});

        addVertex({-.5f, .5f,-.5f}, {1.f,0.f,0.f,1.f}, {0.f,0.f,-1.f});
        addVertex({ .5f, .5f,-.5f}, {0.f,1.f,0.f,1.f}, {0.f,0.f,-1.f});
        addVertex({ .5f,-.5f,-.5f}, {0.f,0.f,1.f,1.f}, {0.f,0.f,-1.f});

        for( auto& v: mVertices )
        {
            v *= dimensions;
            v += center;
        }
    }

    Cube::~Cube()
    {
    }
} }
