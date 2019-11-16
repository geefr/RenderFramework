#include "engine/engine.h"

#ifndef _WIN32
  #define GL_GLEXT_PROTOTYPES
#endif
#include <GLFW/glfw3.h>

#include "dataformats/vector/primitives/cube.h"

#include "cubescene.h"

// TODO: Fix this..
#ifndef M_PI
# define M_PI 3.14159265358979323846
#endif

using namespace renderframework;
Engine engine;
vec3 viewRotDelta{ 0.01f, 0.015f, 0.02f };

[[noreturn]] void quit(std::string msg)
{
    throw std::runtime_error(msg);
}

void checkGlError()
{
    auto error = glGetError();
    switch(error)
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

void keyCallback(GLFWwindow* window, int key, [[maybe_unused]] int scancode, int action, [[maybe_unused]] int modes)
{
    if( action == GLFW_PRESS )
    {
        switch(key)
        {
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(window, GLFW_TRUE);
                break;
            case GLFW_KEY_1:
                viewRotDelta[0] = M_PI / 360.0f;
                break;
            case GLFW_KEY_2:
                viewRotDelta[0] = -M_PI / 360.0f;
                break;
            case GLFW_KEY_3:
                viewRotDelta[2] = M_PI / 360.0f;
                break;
            case GLFW_KEY_4:
                viewRotDelta[2] = -M_PI / 360.0f;
                break;
        }
    }
    else if( action == GLFW_RELEASE )
    {
        switch(key)
        {
            case GLFW_KEY_1:
            case GLFW_KEY_2:
                viewRotDelta[0] = 0.0f;
                break;
            case GLFW_KEY_3:
            case GLFW_KEY_4:
                viewRotDelta[2] = 0.0f;
                break;
        }
    }
}

void mouseButtonCallback(GLFWwindow* window, int button, int action,[[maybe_unused]]  int mods)
{
    if( action != GLFW_PRESS ) return;

    double cx, cy;
    glfwGetCursorPos(window, &cx, &cy);
/*
    float x = static_cast<float>(cx) / engine.windowSize[0];
    float y = static_cast<float>(cy) / engine.windowSize[1];

    x = engine.viewExtent[0] + (x * (engine.viewExtent[1] - engine.viewExtent[0]));
    y = engine.viewExtent[2] + (y * (engine.viewExtent[3] - engine.viewExtent[2]));

    switch(button)
    {
        case GLFW_MOUSE_BUTTON_1:
            engine.viewCenter[0] += x;
            engine.viewCenter[1] += y;
            break;
    }
*/
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    yoffset *= -1.0f;
    if( yoffset < 0.0 ) yoffset = 1.0 - (-yoffset / 20.0);
    else yoffset = 1.0 + (yoffset / 20.0);

    /*
    engine.viewExtent[0] *= static_cast<float>(yoffset);
    engine.viewExtent[1] *= static_cast<float>(yoffset);
    engine.viewExtent[2] *= static_cast<float>(yoffset);
    engine.viewExtent[3] *= static_cast<float>(yoffset);
    */
}


void loadScene1(std::map<std::string,std::shared_ptr<nodes::Node>>& scenes) {

}

void loadScene2(std::map<std::string,std::shared_ptr<nodes::Node>>& scenes) {
  // A simple spinning cube (Different colour, spinning the other way
  std::shared_ptr<nodes::Node> scene(new nodes::Node());

  std::shared_ptr<nodes::MeshNodeDA> mesh(new nodes::MeshNodeDA);
  mesh->meshes().emplace_back(new vector::Cube({0.f,0.f,0.f}, {1.f,1.f,1.f}));
  mesh->shader() = engine.mShaders["phong"];
  mesh->material() = engine.mMaterials["ruby"];
  mesh->rotationDelta() = {0.f,-1.f,0.f};
  scenes["scene2"] = mesh;
}

int main()
{
try
{
    glfwSetErrorCallback(errorCallback);
    if( !glfwInit() ) quit("Failed to init glfw");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    auto window = glfwCreateWindow(800, 600, "Render Engine OpenGL Test", nullptr, nullptr);
    if( !window ) quit("Failed to init window");

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetScrollCallback(window, scrollCallback);

    glfwSwapInterval(1);

    engine.init();

    std::map<std::string,std::shared_ptr<Scene>> scenes;

    scenes["scene1"].reset(new CubeScene(engine, "emerald", {0.f,1.f,0.f}));
    scenes["scene2"].reset(new CubeScene(engine, "ruby", {0.f,-1.f,0.f}));

    auto lastSceneChange = std::chrono::steady_clock::now();
    const auto sceneChangeTime = std::chrono::seconds(2);

    engine.changeScene(scenes["scene1"]);

    // Enough light to make things /slightly/ prettier
    engine.light.mAmbient = {.3f,.3f,.3f};
    engine.light.mDiffuse = {.5f,.5f,.5f};
    engine.light.mSpecular = {.1f,.1f,.1f};
    engine.light.mPosition = {0.f,10.f,0.f};

    while(!glfwWindowShouldClose(window))
    {
        // Pet the event doggie so it barks at our callbacks
        glfwPollEvents();

        auto currentTime = std::chrono::steady_clock::now();
        if( currentTime > lastSceneChange + sceneChangeTime ) {
          lastSceneChange = currentTime;

          if( engine.scene() == scenes["scene1"] )
            engine.changeScene(scenes["scene2"]);
          else
            engine.changeScene(scenes["scene1"]);
        }
        // Hack, should use framebuffersizecallback ;)
        auto width = 0;
        auto height = 0;
        glfwGetFramebufferSize(window, &width, &height);

        engine.viewMatrixLookAt(
          {0.f,1.f,-1.f},
          {0.f,0.f,0.f},
          {0.f,1.f,0.f});
        engine.projectionMatrixPerspective(90, width / height, 0.01f, 10.0f);

        engine.loop(width, height);

        glfwSwapBuffers(window);
    }

    // TODO: Being naughty here and not deleting the OpenGL resources :/

    glfwDestroyWindow(window);
    glfwTerminate();
}
catch(const std::exception& e)
{
    std::cerr << e.what() << std::endl;
    return 1;
}
catch(...)
{
    std::cerr << "Unknown exception" << std::endl;
    return 1;
}

return 0;
}
