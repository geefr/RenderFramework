#ifndef MODELNODE_H
#define MODELNODE_H

#include "node.h"
#include "dataformats/vector/vector.h"
#include "engine/shaderprogram.h"
#include "engine/materials/material.h"

namespace renderframework { namespace nodes {

    /**
     * A node which renders one or more meshes via glDrawArrays
     */
    class MeshNodeDA : public Node
    {
        using Meshes = std::vector<std::shared_ptr<vector::Vector>>;
    public:
        MeshNodeDA();
        virtual ~MeshNodeDA();

        // Mesh accessors
        Meshes& meshes();

        std::shared_ptr<ShaderProgram>& shader();
        std::shared_ptr<materials::Material>& material();

        // Render this node and any children
        void doInit() final override;
        void doUpload() final override;
        void doRender() final override;
    private:
        uint32_t mNumVerts = 0;
        Meshes mMeshes;
        GLuint mVBO = 0;
        std::shared_ptr<ShaderProgram> mShader;
        std::shared_ptr<materials::Material> mMaterial;
    };
} }

#endif // MODELNODE_H
