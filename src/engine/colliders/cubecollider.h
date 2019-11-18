#ifndef CUBECOLLIDER_H
#define CUBECOLLIDER_H

#include <glm/vec3.hpp>

#include "collider.h"

namespace renderframework { namespace colliders {
  /// Axis-Aligned box collider
  class CubeCollider : public Collider {
  public:
    CubeCollider( glm::vec3 center, glm::vec3 dimensions );
    virtual ~CubeCollider();

    virtual bool intersect( const CubeCollider& other );
  private:
    bool overlap( float minA, float maxA, float minB, float maxB );
    glm::vec3 mCenter;
    glm::vec3 mDimensions;
  };
} }

#endif
