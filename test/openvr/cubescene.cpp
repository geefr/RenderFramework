#include "cubescene.h"

#include "engine/engine.h"
#include "engine/nodes/meshnodeda.h"

using namespace renderframework;

CubeScene::CubeScene(Engine& engine, renderframework::vrhell::Env& halp)
  : mHalp(halp)
{
  {
    std::shared_ptr<nodes::MeshNodeDA> cubeNode(new nodes::MeshNodeDA());
    cubeNode->meshes().emplace_back(new vector::Cube({ 0.f,1.f,0.f }, { .5f, .5f, .5f }));
    cubeNode->shader() = engine.mShaders["phong"];
    cubeNode->material() = engine.mMaterials["gold"];
    mNode->children().push_back(cubeNode);
  }

  mLeftHandNode.reset(new nodes::MeshNodeDA());
  mLeftHandNode->meshes().emplace_back(new vector::Cube({0.f,0.f,0.f}, {.05f,.05f,.5f} ));
  mLeftHandNode->shader() = engine.mShaders["phong"];
  mLeftHandNode->material() = engine.mMaterials["emerald"];
  mLeftHandNode->rotationDelta() = { .0f,.0f,1.f };
  mNode->children().push_back(mLeftHandNode);

  mRightHandNode.reset(new nodes::MeshNodeDA);
  mRightHandNode->meshes().emplace_back(new vector::Cube({ 0.f,0.f,0.f }, { .05f,.05f,0.5f }));
  mRightHandNode->shader() = engine.mShaders["phong"];
  mRightHandNode->material() = engine.mMaterials["ruby"];
  mRightHandNode->rotationDelta() = { .0f,.0f,-1.f };
  mNode->children().push_back(mRightHandNode);

  mNode->init();
  mNode->upload();
}

void CubeScene::update( Engine& engine, std::chrono::time_point<std::chrono::high_resolution_clock> currentT, double deltaT ) {
  mNode->update(deltaT);

  // Positions the controllers
  auto left = mHalp.leftHand();
  auto right = mHalp.rightHand();
  if (left) mLeftHandNode->userModelMatrix(left->deviceToAbsoluteMatrix());
  if (right) mRightHandNode->userModelMatrix(right->deviceToAbsoluteMatrix());

  // Update some stuff
  /*
  // TODO: Lights should be nodes shouldn't they!
  mat4x4 lightTransform(1.0f);
  lightTransform = glm::rotate(lightTransform, 0.1f, vec3(0.f,1.f,0.f));
  engine.light.mPosition = vec4(engine.light.mPosition, 1.0f) * lightTransform;
  */
  engine.light.mPosition = right->positionAbsolute();
}
