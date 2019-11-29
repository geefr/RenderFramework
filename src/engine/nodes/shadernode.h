#ifndef SHADERNODE_H
#define SHADERNODE_H

#include "node.h"
#include "engine/shaderprogram.h"
#include "engine/materials/material.h"

namespace renderframework { namespace nodes {

    /**
     * A node which activates a shader program
     *
     * All child nodes will be rendered with the provided
     * shader/material.
     *
     * Other ShaderNodes may be added as children, these will
     * be activated in turn and used for their child nodes
     */
    class ShaderNode : public Node
    {
    public:
        ShaderNode();
        virtual ~ShaderNode() override;

        std::shared_ptr<ShaderProgram>& shader();
        std::shared_ptr<materials::Material>& material();

        // Render this node and any children
        void doInit() override;
        void doUpload() override;
        virtual void doPreChildRender() override;
    private:
        std::shared_ptr<ShaderProgram> mShader;
        std::shared_ptr<materials::Material> mMaterial;
    };
} }

#endif
