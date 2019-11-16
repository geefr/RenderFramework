#include "cubescene.h"

#include "engine/engine.h"
#include "engine/nodes/meshnodeda.h"

#include <algorithm>

using namespace renderframework;

CubeScene::CubeScene(Engine& engine, renderframework::vrhell::Env& halp)
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

  mTargetNode.reset(new nodes::MeshNodeDA());
  mTargetNode->meshes().emplace_back(new vector::Cube({ 0.f,0.f,10.f }, { 1.f, 1.f, .001f }));
  mTargetNode->shader() = engine.mShaders["phong"];
  mTargetNode->material() = engine.mMaterials["ruby"];
  mNode->children().push_back(mTargetNode);
  setTargetPos();

  mBullets.reset(new nodes::Node());

  mNode->init();
  mNode->upload();
}

void CubeScene::update( Engine& engine, std::chrono::time_point<std::chrono::high_resolution_clock> currentT, double deltaT ) {
  mNode->update(deltaT);

  // Cull the scene graph
  {
    auto& toCull = mBullets->children();
    toCull.erase(std::remove_if(toCull.begin(), toCull.end(),
      [](std::shared_ptr<nodes::Node>& node) {
      return node->translation().x > 10.f ||
             node->translation().x < -10.f ||
             node->translation().y > 10.f ||
             node->translation().y < -10.f ||
             node->translation().z > 10.f ||
             node->translation().z < -10.f;
    }), toCull.end());
  }
  // Check collisions between bullets and target
  /*
  {
    auto& targetCube = mTargetNode->meshes().front();
    for( auto b : mBullets->children() ) {
      auto bullet = std::dynamic_pointer_cast<nodes::MeshNodeDA>(b);


      if( bullet->boundsTouching(mTargetNode) ) {
        lastTargetMove = std::chrono::high_resolution_clock::now();
        targetDelta *= 0.9f;
        setTargetPos();
      }
    }
  }
  */
}

void CubeScene::updateVR(Engine& engine, renderframework::vrhell::Env& halp) {
  // Positions the controllers
  auto left = halp.leftHand();
  auto right = halp.rightHand();
  if (left) mLeftHandNode->userModelMatrix(left->deviceToAbsoluteMatrix());
  if (right) mRightHandNode->userModelMatrix(right->deviceToAbsoluteMatrix());

  if( right ) {
    if( right->mButtonPressed[vr::k_EButton_SteamVR_Trigger] ) {
      auto delta = ((double)(std::chrono::high_resolution_clock::now() - lastShot).count()) / 1.0e9;
      if( delta > shotDelta ) {
        lastShot = std::chrono::high_resolution_clock::now();

        fireBullet( engine, right );
      }
    }
  }

  // Update some stuff
  /*
  // TODO: Lights should be nodes shouldn't they!
  mat4x4 lightTransform(1.0f);
  lightTransform = glm::rotate(lightTransform, 0.1f, vec3(0.f,1.f,0.f));
  engine.light.mPosition = vec4(engine.light.mPosition, 1.0f) * lightTransform;
  */
  engine.light.mPosition = right->positionAbsolute();
}

void CubeScene::setTargetPos() {

  auto low = -10.f;
  auto high = 10.f;
  auto x = low + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(high - low)));
  auto y = low + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(high - low)));

  mTargetNode->translation() = { x,y,10.f };
}

void CubeScene::fireBullet( renderframework::Engine& engine, renderframework::vrhell::Controller* right ) {
  auto startPos = right->positionAbsolute();

  std::shared_ptr<nodes::MeshNodeDA> bullet(new nodes::MeshNodeDA());
  bullet->meshes().emplace_back(new vector::Cube(startPos, {.01f,.01f,.01f} ));
  bullet->shader() = engine.mShaders["phong"];
  bullet->material() = engine.mMaterials["obsidian"];

  // TODO: The engine should just do this if needed during the update phase, or ideally this would be handled in a background thread and such
  bullet->init();
  bullet->upload();

  bullet->translationDelta() = right->directionForward() * 5.f;

  mBullets->children().push_back(bullet);
}
