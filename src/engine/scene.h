#ifndef SCENE_H
#define SCENE_H

#include "nodes/node.h"

#include <chrono>

namespace renderframework {
  class Engine;

 /**
  * A scene, to be displayed by the engine
  * The engine can display one at a time
  * The application can switch scenes at any time via Engine::changeScene
  *
  * The application must inherit from this class in order to provide the engine with scene to render
  */
  class Scene
  {
  public:
    Scene();
    virtual ~Scene() = default;

    virtual void update( Engine& engine, std::chrono::time_point<std::chrono::high_resolution_clock> currentT, double deltaT ) = 0;

    std::shared_ptr<nodes::Node> node() const;
  protected:
    std::shared_ptr<nodes::Node> mNode;
  };
}

#endif
