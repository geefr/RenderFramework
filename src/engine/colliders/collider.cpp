
#include "collider.h"

#include <stdexcept>

namespace renderframework { namespace colliders {
  Collider::~Collider() {}

  bool Collider::intersect( const CubeCollider& ) {
    throw std::runtime_error("Collider::intersect(CubeCollider) not implemented");
  }

} }
