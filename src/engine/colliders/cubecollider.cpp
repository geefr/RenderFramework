#include "cubecollider.h"

namespace renderframework { namespace colliders {

  CubeCollider::CubeCollider( glm::vec3 center, glm::vec3 dimensions )
    : mCenter(center), mDimensions(dimensions)
  {}

  CubeCollider::~CubeCollider() {}

  bool CubeCollider::intersect( const CubeCollider& other ) {

    if( overlap( mCenter.x - mDimensions.x, mCenter.x + mDimensions.x,
          other.mCenter.x - other.mDimensions.x, other.mCenter.x + other.mDimensions.x) &&
        overlap( mCenter.y - mDimensions.y, mCenter.y + mDimensions.y,
                  other.mCenter.y - other.mDimensions.y, other.mCenter.y + other.mDimensions.y) &&
        overlap( mCenter.z - mDimensions.z, mCenter.z + mDimensions.z,
                  other.mCenter.z - other.mDimensions.z, other.mCenter.z + other.mDimensions.z))
    {
      return true;
    }
    return false;
  }

  bool CubeCollider::overlap( float minA, float maxA, float minB, float maxB ) {
    if( minA > maxB ) return false;
    if( maxA < minB ) return false;
    return true;
  }

} }
