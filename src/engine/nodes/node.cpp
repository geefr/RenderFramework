#include "node.h"

namespace renderframework { namespace nodes {
    Node::Node()
    {
    }

    Node::~Node()
    {
    }


    Node::Children& Node::children() { return mChildren; }
    vec3& Node::scale() { return mScale; }
    vec3& Node::rotation() { return mRot; }
    vec3& Node::translation() { return mTrans; }

    mat4x4 Node::matrix() const
    {
        mat4x4 m(1.0);
        m = glm::scale(m, mScale);
        m = rotate(m, mRot[0], vec3(1.f,0.f,0.f));
        m = rotate(m, mRot[1], vec3(0.f,1.f,0.f));
        m = rotate(m, mRot[2], vec3(0.f,0.f,1.f));
        m = glm::translate(m, mTrans);
        return m;
    }

    void Node::init()
    {
        doInit();
        for( auto& c: mChildren ) c->init();
    }

    void Node::upload()
    {
        doUpload();
        for( auto& c: mChildren ) c->upload();
    }

    void Node::render(mat4x4 modelMat, mat4x4 viewMat, mat4x4 projMat)
    {
        // This node's transformation matrix
        //mat4x4 nodeMat(1.0f);
        modelMat = glm::translate(modelMat, mTrans);
        modelMat = glm::rotate(modelMat, mRot.x, vec3(1.f,0.f,0.f));
        modelMat = glm::rotate(modelMat, mRot.y, vec3(0.f,1.f,0.f));
        modelMat = glm::rotate(modelMat, mRot.z, vec3(0.f,0.f,1.f));
        modelMat = glm::scale(modelMat, mScale);



        // Apply any previous transforms
        // TODO: This should work? We don't need to now traverse up to the root node and apply them in that order? Matrices are hard :/
        // nodeMat *= modelMat;

        doRender(modelMat, viewMat, projMat);
        for( auto& c: mChildren ) c->render(modelMat, viewMat, projMat);
    }

    void Node::doInit() {}
    void Node::doUpload() {}
    void Node::doRender(mat4x4 modelMat, mat4x4 viewMat, mat4x4 projMat) {}
} }
