#include "meshnodeda.h"
#include "common.h"

#include <GL/gl.h>

#include <vector>
#include <stdexcept>

using namespace renderframework;

namespace renderframework { namespace nodes {

    MeshNodeDA::MeshNodeDA()
    {
    }

    MeshNodeDA::~MeshNodeDA()
    {
        // This of course assumes the context is current so...
        glDeleteBuffers(1,&mVBO);
    }

    MeshNodeDA::Meshes& MeshNodeDA::meshes() { return mMeshes; }
    std::shared_ptr<ShaderProgram>& MeshNodeDA::shader() { return mShader; }
    std::shared_ptr<materials::Material>& MeshNodeDA::material() { return mMaterial; }

    void MeshNodeDA::doInit()
    {
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
            std::vector<VertexDef> meshBuf;
            auto vertices = mesh->vertices();
            auto colors = mesh->vertexColors(); // TODO: Per-vertex colours ignored in shaders right now
            auto normals = mesh->normals();

            for( auto& v: vertices )
            {
                meshBuf.push_back({v, {}, {}});
            }
            for( auto i=0u;i < colors.size(); ++i )
            {
                meshBuf[i].color = colors[i];
            }
            for( auto i=0u;i < normals.size(); ++i )
            {
                meshBuf[i].normal = normals[i];
            }
            buffer.insert(std::end(buffer), std::begin(meshBuf), std::end(meshBuf));
        }
        mNumVerts = buffer.size();

        // Buffer upload
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

    void MeshNodeDA::doRender(mat4x4 modelMat, mat4x4 viewMat, mat4x4 projMat)
    {
        glUseProgram(mShader->id());
        // TODO: Matrix uniforms and stuff
        mMaterial->setUniforms(mShader);
        // TODO: Lighting? How's that going to work?

        mat4x4 mvp = modelMat;
        mvp *= viewMat;
        mvp *= projMat;

        glUniformMatrix4fv(mShader->uniform("modelViewProjectionMatrix"), 1, GL_FALSE, value_ptr(mvp));
        glUniformMatrix4fv(mShader->uniform("modelMatrix"), 1, GL_FALSE, value_ptr(modelMat));
        glUniformMatrix4fv(mShader->uniform("viewMatrix"), 1, GL_FALSE, value_ptr(viewMat));
        glUniformMatrix4fv(mShader->uniform("projectionMatrix"), 1, GL_FALSE, value_ptr(projMat));

        // TODO: Need to use triangles if the shader program doesn't use tesselation shaders
        // TODO: Patch vertices should be defined as part of shader, for now expect that it's already been setup
        glDrawArrays(GL_PATCHES, 0, mNumVerts);
    }
} }
