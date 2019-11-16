#include "scene.h"

#include "engine.h"

namespace renderframework {
  Scene::Scene() : mNode(new nodes::Node()) {}
  std::shared_ptr<nodes::Node> Scene::node() const { return mNode; }
}
