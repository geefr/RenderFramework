#define GL_GLEXT_PROTOTYPES
#include <GLFW/glfw3.h>

#include "engine/engine.h"
#include "dataformats/vector/primitives/cube.h"

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

int main()
{
try
{
    if( !glfwInit() ) quit("Failed to init glfw");
    glfwSetErrorCallback(errorCallback);

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

    // Generate the vertex data and such
    engine.mNode.reset(new nodes::Node());
    {
        std::shared_ptr<nodes::MeshNodeDA> cubeNode(new nodes::MeshNodeDA());
        cubeNode->meshes().emplace_back(new vector::Cube({2.f,.0f,-2.f}, {2.0f, 2.0f, 2.0f}));
        cubeNode->meshes().emplace_back(new vector::Cube({-2.f,.0f,-2.f}, {2.0f, 2.0f, 2.0f}));
        cubeNode->meshes().emplace_back(new vector::Cube({2.f,.0f,2.f}, {2.0f, 2.0f, 2.0f}));
        cubeNode->meshes().emplace_back(new vector::Cube({-2.f,.0f,2.f}, {2.0f, 2.0f, 2.0f}));
        cubeNode->shader() = engine.mShaders["phong"];
        cubeNode->material() = engine.mMaterials["yellow rubber"];
        cubeNode->translation() = vec3(0.f,2.f,0.f);
        cubeNode->rotation() = vec3(.1f,.1f,.1f);
        cubeNode->scale() = vec3(.7f,.7f,.7f);
        cubeNode->rotationDelta() = vec3(0.0f, 0.02f, 0.0f);
        engine.mNode->children().push_back(cubeNode);
    }

    std::shared_ptr<nodes::MeshNodeDA> cubeNode(new nodes::MeshNodeDA());
    cubeNode->scaleDelta() = {1.2f,1.2f,1.2f};

    {
        cubeNode->meshes().emplace_back(new vector::Cube({2.f,.0f,-2.f}, {2.0f, 2.0f, 2.0f}));
        cubeNode->meshes().emplace_back(new vector::Cube({-2.f,.0f,-2.f}, {2.0f, 2.0f, 2.0f}));
        cubeNode->meshes().emplace_back(new vector::Cube({2.f,.0f,2.f}, {2.0f, 2.0f, 2.0f}));
        cubeNode->meshes().emplace_back(new vector::Cube({-2.f,.0f,2.f}, {2.0f, 2.0f, 2.0f}));
        cubeNode->shader() = engine.mShaders["phong"];
        cubeNode->material() = engine.mMaterials["ruby"];
        cubeNode->translation() = vec3(0.f,-2.f,0.f);
        cubeNode->rotationDelta() = vec3(0.0f, -0.01f, 0.0f);
        engine.mNode->children().push_back(cubeNode);
    }
    //engine.mNode->translation() = vec3(0.f,0.f,-5.f);
engine.mNode->scaleDelta() = {0.99f,0.99f,0.99f};
//engine.mNode->scale() = {0.2f,0.2f,0.2f};

    engine.init2();

    while(!glfwWindowShouldClose(window))
    {
        // Pet the event doggie so it barks at our callbacks
        glfwPollEvents();

        std::cerr << "scale: " << engine.mNode->scale().x;
        if( engine.mNode->scale().x > 0.8f )
        {
            engine.mNode->scaleDelta() = {0.98f,0.98f,0.98f};
        }
        else if( engine.mNode->scale().x < 0.001f )
        {
            engine.mNode->scaleDelta() = {1.02f,1.02f,1.02f};
        }

        std::cerr << "cubeNode scale: " << engine.mNode->scale().x;
        if( cubeNode->scale().x > 0.8f )
        {
            cubeNode->scaleDelta() = {0.9f,0.9f,0.9f};
        }
        else if( cubeNode->scale().x < 0.001f )
        {
            cubeNode->scaleDelta() = {1.1f,1.1f,1.1f};
        }



        engine.mNode->rotationDelta() = viewRotDelta;

        // Hack, should use framebuffersizecallback ;)
        auto width = 0;
        auto height = 0;
        glfwGetFramebufferSize(window, &width, &height);
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
