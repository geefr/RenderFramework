#ifndef MESHNODEDI_H
#define MESHNODEDI_H

#include "node.h"
#include "dataformats/vector/vector.h"
#include "engine/shaderprogram.h"
#include "engine/materials/material.h"

namespace renderframework { namespace nodes {

    /**
     * A node which renders one or more meshes via glDrawElements
     */
    class MeshNodeDI : public Node
    {
        using Meshes = std::vector<std::shared_ptr<vector::Vector>>;
    public:
        MeshNodeDI();
        virtual ~MeshNodeDI();

        // Mesh accessors
        Meshes& meshes();

        std::shared_ptr<ShaderProgram>& shader();
        std::shared_ptr<materials::Material>& material();

        // Render this node and any children
        void doInit() override;
        void doUpload() override;
        void doRender(mat4x4 nodeMat, mat4x4 viewMat, mat4x4 projMat) override;
    private:
        uint32_t mNumVerts = 0;
        Meshes mMeshes;
        GLuint mVAO = 0;
        GLuint mVBO = 0;
        GLuint mIBO = 0;
        std::shared_ptr<ShaderProgram> mShader;
        std::shared_ptr<materials::Material> mMaterial;
    };
} }

#endif
