#ifndef CUBESCENE_H
#define CUBESCENE_H

#include "engine/engine.h"
#include "engine/scene.h"

class CubeScene : public renderframework::Scene
{
public:
  CubeScene(renderframework::Engine& engine, std::string material, glm::vec3 rotDelta);
  virtual ~CubeScene() = default;
  virtual void update( renderframework::Engine& engine, std::chrono::time_point<std::chrono::high_resolution_clock> currentT, double deltaT ) final override;
};

#endif
