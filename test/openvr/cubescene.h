#ifndef CUBESCENE_H
#define CUBESCENE_H

#include "engine/engine.h"
#include "engine/scene.h"
#include "engine/nodes/meshnodeda.h"
#include "engine/vr/env.h"

class CubeScene : public renderframework::Scene
{
public:
  CubeScene(renderframework::Engine& engine, renderframework::vrhell::Env& halp);
  virtual ~CubeScene() = default;
  virtual void update( renderframework::Engine& engine, std::chrono::time_point<std::chrono::high_resolution_clock> currentT, double deltaT ) final override;
private:
  std::shared_ptr<renderframework::nodes::MeshNodeDA> mLeftHandNode;
  std::shared_ptr<renderframework::nodes::MeshNodeDA> mRightHandNode;
  renderframework::vrhell::Env& mHalp;
};

#endif
