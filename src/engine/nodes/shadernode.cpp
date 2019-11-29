#include "shadernode.h"

using namespace renderframework;

namespace renderframework { namespace nodes {

    ShaderNode::ShaderNode()
    {
    }

    ShaderNode::~ShaderNode()
    {
    }

    std::shared_ptr<ShaderProgram>& ShaderNode::shader() { return mShader; }
    std::shared_ptr<materials::Material>& ShaderNode::material() { return mMaterial; }

    void ShaderNode::doInit()
    {
        if( !mShader ) throw std::runtime_error("Unable to init shader node without shader");
        if( !mMaterial ) throw std::runtime_error("Unable to init shader node without material");
        // Ensure the IDs for our uniforms are fetched for the shader
        mShader->id();
        mMaterial->registerUniforms(mShader);
    }

    void ShaderNode::doUpload()
    {
      // Nothing to do here
    }

    void ShaderNode::doPreChildRender()
    {
      // Activate shader program before each child is rendered
      // This ensures if a child node switches the shader the next
      // direct child won't be affected by it
      glUseProgram(mShader->id());
      mMaterial->setUniforms(mShader);
    }
} }
