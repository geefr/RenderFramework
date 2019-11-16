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
  void updateVR(renderframework::Engine& engine, renderframework::vrhell::Env& halp);

  void setTargetPos();
  void fireBullet( renderframework::Engine& engine, renderframework::vrhell::Controller* right );

private:
  std::shared_ptr<renderframework::nodes::MeshNodeDA> mLeftHandNode;
  std::shared_ptr<renderframework::nodes::MeshNodeDA> mRightHandNode;
  std::shared_ptr<renderframework::nodes::MeshNodeDA> mTargetNode;
  std::shared_ptr<renderframework::nodes::Node> mBullets;


  // TODO: This is hax
  std::chrono::time_point<std::chrono::high_resolution_clock> lastTargetMove = std::chrono::high_resolution_clock::now();
  float targetDelta = 5.f;
  std::chrono::time_point<std::chrono::high_resolution_clock> lastShot = std::chrono::high_resolution_clock::now();
  float shotDelta = 0.25;
};

#endif
