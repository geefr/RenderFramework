#include "cubescene.h"

#include "engine/engine.h"
#include "engine/nodes/meshnodeda.h"

using namespace renderframework;

CubeScene::CubeScene(Engine& engine, std::string material, glm::vec3 rotDelta)
{
  std::shared_ptr<nodes::MeshNodeDA> mesh(new nodes::MeshNodeDA);
  mesh->meshes().emplace_back(new vector::Cube({0.f,0.f,0.f}, {1.f,1.f,1.f}));
  mesh->shader() = engine.mShaders["phong"];
  mesh->material() = engine.mMaterials[material];
  mesh->rotationDelta() = rotDelta;
  mNode->children().emplace_back(mesh);

  mNode->init();
  mNode->upload();
}

void CubeScene::update( Engine& engine, std::chrono::time_point<std::chrono::high_resolution_clock> currentT, double deltaT ) {
  mNode->update(deltaT);
}
