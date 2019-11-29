#ifndef ENGINE_H
#define ENGINE_H

#include <iostream>
#include <exception>
#include <stdexcept>
#include <fstream>
#include <vector>
#include <chrono>
#include <map>

#include "dataformats/raster/png/rfpng.h"
#include "dataformats/vector/vector.h"
#include "dataformats/vector/primitives/cube.h"
#include "common.h"
#include "shaderprogram.h"
#include "materials/phongmaterialbare.h"
#include "nodes/meshnodeda.h"
#include "light.h"
#include "framebuffer.h"
#include "defaultframebuffer.h"

#include "scene.h"

namespace renderframework
{
    class Engine
    {
    public:
        Engine();
        void init();

        /**
         * Update view matrix
         *
         * eyePos will be stored and passed through to shader uniforms
         */
        void viewMatrixLookAt(vec3 eyePos, vec3 target, vec3 up);
        /**
         * Update view matrix explicitly
         *
         * eyePos in shaders will be extracted from this matrix
         */
        void viewMatrix(mat4x4 viewMat, vec3 eyePos);

        mat4x4 viewMatrix() const;

        void projectionMatrixOrtho(vec4 orthoSpace);
        void projectionMatrixPerspective(float fov, float aspect, float nearPlane, float farPlane);
        void projectionMatrix(mat4x4 proj);
        
        mat4x4 projectionMatrix() const;

        /**
         * Update the engine/scene graph state
         *
         * Must be called once per application loop
         * Must be called before render (assuming you want to update positions every render)
         */
        void update();

        /**
         * Perform collision checks
         * TODO: This is slow, use with care!
         */
        std::multimap<std::shared_ptr<nodes::Node>, std::shared_ptr<nodes::Node>> collisions() const;

        /**
         * Render the scene
         *
         * If no framebuffer is provided will render to FBO 0
         */
        void render(float width, float height, const FrameBuffer* framebuffer = nullptr);
        void render(const FrameBuffer* framebuffer);
        /// @deprecated, see Engine::render
        void loop( float width, float height );

        /**
         * Transition to a new scene
         */
        void changeScene( std::shared_ptr<Scene> scene );
        // void changeScene( std::shared_ptr<Scene> scene, TODO: Scene transition class here? What's the best way to implement these? );
        std::shared_ptr<Scene> scene() const;

        [[noreturn]] static void quit(std::string msg);

        // Engine options

        /// Enable/Disable depth testing
        void depthTest(bool enable);

        /// Enable/Disable alpha blending
        /// Default enabled, src alpha/1 - src alpha
        /// TODO: Allow different blend functions here, but we probably won't ever use them
        void alphaBlending(bool enable);

        /// Clear colour
        /// Default black
        void clearColor(vec4 c);

        /// MSAA enable
        void MSAA(bool enable);

        float secondsSinceInit() const;

        float width() const;
        float height() const;

        // Rendering stuff and hacks below here
        std::map<std::string, std::shared_ptr<materials::PhongMaterialBare>> mMaterials;
        std::map<std::string, std::shared_ptr<ShaderProgram>> mShaders;
        Light light;

        /// The root directory for the engines data
        /// If not set will be read from RENDERFRAMEWORK_ROOT environment variable
        std::string mDataDir;
    private:
        void checkGlError();
        GLuint loadTexture( std::string fileName );
        void setVertexAttribPointers(GLint vertCoordAttrib, GLint texCoordAttrib, GLint vertColorAttrib);
        void printMat4x4(mat4x4 mat, std::string name);

        GLuint vao;
        GLuint catTexture;

        bool mEnableDepthTest = true;
        bool mEnableAlpha = true;
        vec4 mClearColor = {0.f,0.f,0.f,1.f};
        bool mMSAA = true;
        float mWidth = 0.f;
        float mHeight = 0.f;

        std::chrono::time_point<std::chrono::high_resolution_clock> mTimeStart;
        std::chrono::time_point<std::chrono::high_resolution_clock> mTimeCurrent;

        vec3 mEyePos;
        mat4x4 mModelMatrix;
        mat4x4 mViewMatrix;
        mat4x4 mProjectionMatrix;

        vec4 mOrthoSpace = { -10.f,10.f,-10.f,10.f };

        DefaultFrameBuffer mDefaultFramebuffer;

        std::shared_ptr<Scene> mScene;
    };
}

#endif // ENGINE_H
