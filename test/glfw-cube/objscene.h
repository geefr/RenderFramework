#ifndef OBJSCENE_H
#define OBJSCENE_H

#include "engine/engine.h"
#include "engine/scene.h"

class ObjScene : public renderframework::Scene
{
public:
  ObjScene(renderframework::Engine& engine, glm::vec3 rotDelta);
  virtual ~ObjScene() = default;
  virtual void update( renderframework::Engine& engine, std::chrono::time_point<std::chrono::high_resolution_clock> currentT, double deltaT ) final override;
};

#endif
