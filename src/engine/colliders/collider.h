#ifndef COLLIDER_H
#define COLLIDER_H

namespace renderframework { namespace colliders {
  class CubeCollider;

  class Collider {
  public:
    virtual ~Collider();

    virtual bool intersect( const CubeCollider& other );
  };
} }

#endif
