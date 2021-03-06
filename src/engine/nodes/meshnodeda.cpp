#include "meshnodeda.h"
#include "common.h"

#include <vector>
#include <stdexcept>
#include <algorithm>

using namespace renderframework;

namespace renderframework { namespace nodes {

    MeshNodeDA::MeshNodeDA()
    {
    }

    MeshNodeDA::~MeshNodeDA()
    {
        // This of course assumes the context is current so...
        glDeleteBuffers(1,&mVBO);
        glDeleteVertexArrays(1,&mVAO);
    }

    MeshNodeDA::Meshes& MeshNodeDA::meshes() { return mMeshes; }
    std::shared_ptr<ShaderProgram>& MeshNodeDA::shader() { return mShader; }
    std::shared_ptr<materials::Material>& MeshNodeDA::material() { return mMaterial; }

    void MeshNodeDA::doInit()
    {
        glGenVertexArrays(1, &mVAO);
        glBindVertexArray(mVAO);
        glGenBuffers(1, &mVBO);
        if( !mShader ) throw std::runtime_error("Unable to init mesh node without shader");
        if( !mMaterial ) throw std::runtime_error("Unable to init mesh node without material");
        // Ensure the IDs for our uniforms are fetched for the shader
        mShader->id();
        mMaterial->registerUniforms(mShader);
    }

    void MeshNodeDA::doUpload()
    {
        std::vector<VertexDef> buffer;
        for( auto& mesh : mMeshes )
        {
          if( mesh->usesIndices() ) throw std::runtime_error("MeshNodeDA:doUpload failed: Mesh uses indexed draw, MeshNodeDA only supports non-indexed");

          auto meshBuf = mesh->vertices();
          buffer.insert(std::end(buffer), std::begin(meshBuf), std::end(meshBuf));
        }
        mNumVerts = buffer.size();

        // Buffer upload

        glBindVertexArray(mVAO);
        glBindBuffer(GL_ARRAY_BUFFER, mVBO);
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(buffer.size() * sizeof(VertexDef)), buffer.data(), GL_STATIC_DRAW );

        // TODO: This should probably be common between all the nodes
        // Will need some way to ensure a shader is only used with one kind of buffer?
        // This should probably be on the shader instance itself as it's tied to that mainly
        // Either way something needs to be done here
        glUseProgram(mShader->id());
        mShader->regAttribute("vertCoord", 3, GL_FLOAT, GL_FALSE, sizeof(VertexDef), reinterpret_cast<const void*>(offsetof(VertexDef,coord)));
        // If pointer != null then GL_ARRAY_BUFFER has to be bound
        mShader->regAttribute("vertTexCoord", 2, GL_FLOAT, GL_FALSE, sizeof(VertexDef), reinterpret_cast<const void*>(offsetof(VertexDef,texCoord)));
        mShader->regAttribute("vertTexColor", 4, GL_FLOAT, GL_FALSE, sizeof(VertexDef), reinterpret_cast<const void*>(offsetof(VertexDef,color)));
        mShader->regAttribute("vertNormal", 3, GL_FLOAT, GL_FALSE, sizeof(VertexDef), reinterpret_cast<const void*>(offsetof(VertexDef,normal)));

        mShader->regUniform("modelViewProjectionMatrix");
        mShader->regUniform("modelMatrix");
        mShader->regUniform("viewMatrix");
        mShader->regUniform("projectionMatrix");
    }

    void MeshNodeDA::doRender(mat4x4 nodeMat, mat4x4 viewMat, mat4x4 projMat)
    {
        glBindVertexArray(mVAO);

        glUseProgram(mShader->id());
        // TODO: Matrix uniforms and stuff
        mMaterial->setUniforms(mShader);
        // TODO: Lighting? How's that going to work?
        mat4x4 mvp(1.0f);
        mvp *= nodeMat;
        mvp *= viewMat;
        mvp *= projMat;

        glUniformMatrix4fv(mShader->uniform("modelViewProjectionMatrix"), 1, GL_FALSE, value_ptr(mvp));
        glUniformMatrix4fv(mShader->uniform("modelMatrix"), 1, GL_FALSE, value_ptr(nodeMat));
        glUniformMatrix4fv(mShader->uniform("viewMatrix"), 1, GL_FALSE, value_ptr(viewMat));
        glUniformMatrix4fv(mShader->uniform("projectionMatrix"), 1, GL_FALSE, value_ptr(projMat));

        // TODO: Need to use triangles if the shader program doesn't use tesselation shaders
        // TODO: Patch vertices should be defined as part of shader, for now expect that it's already been setup
        glDrawArrays(GL_PATCHES, 0, mNumVerts);
    }
} }
