#include "objscene.h"

#include "engine/engine.h"
#include "engine/nodes/meshnodedi.h"

#include "dataformats/vector/obj/obj.h"

using namespace renderframework;

ObjScene::ObjScene(Engine& engine, glm::vec3 rotDelta)
{
  std::shared_ptr<nodes::MeshNodeDI> mesh(new nodes::MeshNodeDI);

  mesh->meshes().emplace_back( new vector::Obj("/home/gareth/source/RenderFramework/data/models/space/ships/drone_round_01/drone_round_01.obj") );
  //mesh->meshes().emplace_back( new vector::Obj("/home/gareth/source/RenderFramework/data/models/primitives/cube/cube.obj") );
  mesh->shader() = engine.mShaders["phong"];
  mesh->material() = engine.mMaterials["gold"];
  mesh->rotationDelta() = rotDelta;

  // mesh->translation() = {0.f,0.f,5.f};
  mNode->scale() = {.2f,.2f,.2f};

  mNode->children().emplace_back(mesh);
  mNode->init();
  mNode->upload();
}

void ObjScene::update( Engine& engine, std::chrono::time_point<std::chrono::high_resolution_clock> currentT, double deltaT ) {
  mNode->update(deltaT);
}
