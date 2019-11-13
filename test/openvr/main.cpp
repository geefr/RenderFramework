
#include "engine/engine.h"
#include "engine/framebuffer.h"
#include "engine/framebufferrendertotexture.h"
#include "dataformats/vector/primitives/cube.h"

#include <openvr/openvr.h>
#include <iostream>
#include <iomanip>
#include <list>
#include <algorithm>

#include "engine/vr/env.h"
#include "engine/vr/device.h"
#include "engine/vr/controller.h"

#include <GLFW/glfw3.h>

using namespace renderframework;

[[noreturn]] void quit(std::string msg)
{
  throw std::runtime_error(msg);
}

void checkGlError()
{
  auto error = glGetError();
  switch (error)
  {
  case GL_NO_ERROR: return;
  case GL_INVALID_ENUM: quit("GL_INVALID_ENUM");
  case GL_INVALID_VALUE: quit("GL_INVALID_VALUE");
  case GL_INVALID_OPERATION: quit("GL_INVALID_OPERATION");
  case GL_INVALID_FRAMEBUFFER_OPERATION: quit("GL_INVALID_FRAMEBUFFER_OPERATION");
  case GL_OUT_OF_MEMORY: quit("GL_OUT_OF_MEMORY");
  case GL_STACK_UNDERFLOW: quit("GL_STACK_UNDERFLOW");
  case GL_STACK_OVERFLOW: quit("GL_STACK_OVERFLOW");
  }
}

void errorCallback(int error, const char* description)
{
  std::cerr << "GLFW Error: " << error << ": " << description << std::endl;
}

void setTargetPos( std::shared_ptr<nodes::MeshNodeDA> target ) {

  auto low = -10.f;
  auto high = 10.f;
  auto x = low + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(high - low)));
  auto y = low + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(high - low)));

  target->translation() = { x,y,10.f };
}

void fireBullet( renderframework::Engine& engine, std::shared_ptr<nodes::Node> bullets, renderframework::vrhell::Controller* right ) {
  auto startPos = right->positionAbsolute();


  std::shared_ptr<nodes::MeshNodeDA> bullet(new nodes::MeshNodeDA());
  bullet->meshes().emplace_back(new vector::Cube(startPos, {.01f,.01f,.01f} ));
  bullet->shader() = engine.mShaders["phong"];
  bullet->material() = engine.mMaterials["obsidian"];

  // TODO: The engine should just do this if needed during the update phase, or ideally this would be handled in a background thread and such
  bullet->init();
  bullet->upload();

  bullet->translationDelta() = right->directionForward() * 5.f;

  bullets->children().push_back(bullet);
}

int main(void)
{
  try
  {
    // OpenVR init
    renderframework::vrhell::Env halp(vr::ETrackingUniverseOrigin::TrackingUniverseStanding);
   
    // GLFW/Engine init
    if (!glfwInit()) quit("Failed to init glfw");
    glfwSetErrorCallback(errorCallback);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    bool showDesktopView = false;
    if( !showDesktopView ) glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

    auto width = 800.0f;
    auto height = 600.0f;
    auto window = glfwCreateWindow(width, height, "Render Engine OpenGL Test", nullptr, nullptr);
    if (!window) quit("Failed to init window");

    glfwMakeContextCurrent(window);

    // Always disable VSync for VR
    // TODO: What if the driver forces it back on? would we then be bound to the desktop refresh rate in the hmd? That would suck.
    glfwSwapInterval(0);

    // Init RenderFramework
    Engine engine;
    engine.mDataDir = "/home/gareth/source/RenderFramework/src/";
    engine.init();

    // Generate the vertex data and such
    // TODO: Here's where we would load in the scene, models etc
    engine.mNode.reset(new nodes::Node());
    {
      std::shared_ptr<nodes::MeshNodeDA> cubeNode(new nodes::MeshNodeDA());
      cubeNode->meshes().emplace_back(new vector::Cube({ 0.f,1.f,0.f }, { .5f, .5f, .5f }));
      cubeNode->shader() = engine.mShaders["phong"];
      cubeNode->material() = engine.mMaterials["gold"];
      engine.mNode->children().push_back(cubeNode);
    }

    std::shared_ptr<nodes::MeshNodeDA> targetNode(new nodes::MeshNodeDA());
    {
      targetNode->meshes().emplace_back(new vector::Cube({ 0.f,0.f,10.f }, { 1.f, 1.f, .001f }));
      targetNode->shader() = engine.mShaders["phong"];
      targetNode->material() = engine.mMaterials["ruby"];
      engine.mNode->children().push_back(targetNode);
    }
    setTargetPos(targetNode);

    std::shared_ptr<nodes::MeshNodeDA> leftHandNode(new nodes::MeshNodeDA());
    leftHandNode->meshes().emplace_back(new vector::Cube({0.f,0.f,0.f}, {.05f,.05f,.5f} ));
    leftHandNode->shader() = engine.mShaders["phong"];
    leftHandNode->material() = engine.mMaterials["emerald"];
    leftHandNode->rotationDelta() = { .0f,.0f,1.f };
    engine.mNode->children().push_back(leftHandNode);

    std::shared_ptr<nodes::MeshNodeDA> rightHandNode(new nodes::MeshNodeDA());
    rightHandNode->meshes().emplace_back(new vector::Cube({ 0.f,0.f,0.f }, { .05f,.05f,0.5f }));
    rightHandNode->shader() = engine.mShaders["phong"];
    rightHandNode->material() = engine.mMaterials["ruby"];
    rightHandNode->rotationDelta() = { .0f,.0f,-1.f };
    engine.mNode->children().push_back(rightHandNode);

    // Init RenderFramework (vector buffers/etc uploaded to gpu here)
    // TODO: Would be great if we didn't need to manually call this, maybe some kind of resource upload pipeline/thread
    engine.init2();

    // Create the framebuffers for each eye's output
    // In VR we'll end up rendering once for each eye, and then the desktop view aswell if enabled
    uint32_t hmdWidth=0, hmdHeight=0;
    halp.getRecommendedTargetSize(hmdWidth, hmdHeight);
    std::shared_ptr<FrameBufferRenderToTexture> leftFrameBuffer (new FrameBufferRenderToTexture(hmdWidth, hmdHeight));
    std::shared_ptr<FrameBufferRenderToTexture> rightFrameBuffer(new FrameBufferRenderToTexture(hmdWidth, hmdHeight));

    // A nice lamp in the ceiling
    engine.light.mPosition = { 0.0f,2.0f,-1.0f };
    engine.light.mAmbient = { 0.4f,0.4f,0.4f };
    engine.light.mDiffuse = { 0.6f,0.6f,0.6f };
    engine.light.mSpecular = { 0.3f,0.3f,0.3f};

    // Rendering constants
    auto nearPlane = 0.1f;
    auto farPlane = 30.0;

    std::chrono::time_point<std::chrono::high_resolution_clock> lastTargetMove = std::chrono::high_resolution_clock::now();
    auto targetDelta = 5.f;
    std::chrono::time_point<std::chrono::high_resolution_clock> lastShot = std::chrono::high_resolution_clock::now();
    auto shotDelta = 0.25;

    std::shared_ptr<nodes::Node> bullets( new nodes::Node() );
    engine.mNode->children().push_back(bullets);

    while (!glfwWindowShouldClose(window))
    {
      // Update GLFW - mouse/keyboard
      glfwPollEvents();
      // Update RenderFramework
      // Engine stuff, apply node rotations/etc over time
      engine.update();
      // Update OpenVR
      halp.update();

      // Cull the scene graph
      {
        auto& toCull = bullets->children();
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

      // Update some stuff
      {
        // TODO: Lights should be nodes shouldn't they!
        mat4x4 lightTransform(1.0f);
        lightTransform = glm::rotate(lightTransform, 0.1f, vec3(0.f,1.f,0.f));
        engine.light.mPosition = vec4(engine.light.mPosition, 1.0f) * lightTransform;
      }

      /*
      {
        auto delta = ((double)(std::chrono::high_resolution_clock::now() - lastTargetMove).count()) / 1.0e9;
        if( delta > targetDelta ) {
          lastTargetMove = std::chrono::high_resolution_clock::now();
          setTargetPos( targetNode );
        }
      }*/

      // Check collisions between bullets and target
      {
        auto& targetCube = targetNode->meshes().front();
        for( auto b : bullets->children() ) {
          auto bullet = std::dynamic_pointer_cast<nodes::MeshNodeDA>(b);


          if( bullet->boundsTouching(targetNode) ) {
            lastTargetMove = std::chrono::high_resolution_clock::now();
            targetDelta *= 0.9f;
            setTargetPos( targetNode );
          }
        }
      }

      // Positions the controllers
      auto left = halp.leftHand();
      if( left ) {
        leftHandNode->userModelMatrix(left->deviceToAbsoluteMatrix());
      }

      auto right = halp.rightHand();
      if( right ) {
        if( right->mButtonPressed[vr::k_EButton_SteamVR_Trigger] ) {
          auto delta = ((double)(std::chrono::high_resolution_clock::now() - lastShot).count()) / 1.0e9;
          if( delta > shotDelta ) {
            lastShot = std::chrono::high_resolution_clock::now();

            fireBullet( engine, bullets, right );
          }
        }

        rightHandNode->userModelMatrix(right->deviceToAbsoluteMatrix());
      }

      // Position the HMD
      auto hmd = halp.hmd();
      mat4x4 hmdPoseMat(1.f);
      mat4x4 hmdPoseMatInv(1.f);
      vec3 eyePos(1.f);
      if (hmd)
      {
        hmdPoseMat = hmd->deviceToAbsoluteMatrix();
        hmdPoseMatInv = glm::inverse(hmdPoseMat);
        eyePos = hmd->positionAbsolute();
      }
      
      // Render the left eye
      {
        auto leftProj = halp.getProjectionMatrix(vr::EVREye::Eye_Left, nearPlane, farPlane);
        auto leftEyeFromHeadInv = glm::inverse(halp.getEyeToHeadTransform(vr::EVREye::Eye_Left));
        engine.viewMatrix(leftEyeFromHeadInv * hmdPoseMatInv, eyePos);
        engine.projectionMatrix(leftProj);
        engine.render(leftFrameBuffer.get());
      }
      // Render the left eye
      {
        auto rightProj = halp.getProjectionMatrix(vr::EVREye::Eye_Right, nearPlane, farPlane);
        auto rightEyeFromHeadInv = glm::inverse(halp.getEyeToHeadTransform(vr::EVREye::Eye_Right));
        engine.viewMatrix(rightEyeFromHeadInv * hmdPoseMatInv, eyePos);
        engine.projectionMatrix(rightProj);
        engine.render(rightFrameBuffer.get());
      }

      if( showDesktopView )
      {
        // Render the desktop window
        engine.viewMatrix(hmdPoseMatInv, { hmdPoseMat[3][0], hmdPoseMat[3][1], hmdPoseMat[3][2] });
        engine.projectionMatrixPerspective(120, width / height, 0.2, 10.0);
        engine.render(width, height);

        // VSync must be off for the desktop window here
        // Otherwise we'll limit the hmd framerate to the
        // desktop one!
        glfwSwapBuffers(window);
      }

      // Submit the textures to OpenVR
      // Once submitted the vr compositor will go do its thing with the textures
      // in another thread
      // As long as we don't modify the framebuffers again we're free to do more
      // work here, and maybe that gives us the time to render the desktop window
      // without stalling much
      halp.submitFrames( leftFrameBuffer->textureID(), rightFrameBuffer->textureID() );

      // Wait until the next vr update
      // This is mandatory - without it the frames won't be rendered
      // and we won't get updated tracking data each frame
      halp.waitGetPoses();
    }

    // TODO: Being naughty here and not deleting the OpenGL resources :/
    glfwDestroyWindow(window);
    glfwTerminate();
  } catch ( std::exception& e )
  {
    std::cerr << "ERROR: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
