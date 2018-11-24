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

    vec3& Node::scaleDelta() { return mScaleDelta; }
    vec3& Node::rotationDelta() { return mRotDelta; }
    vec3& Node::translationDelta() { return mTransDelta; }

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

    void Node::render(mat4x4 viewMat, mat4x4 projMat)
    {
        mat4x4 nodeMat(1.0f);
        render(nodeMat, viewMat, projMat);
    }

    void Node::render(mat4x4 nodeMat, mat4x4 viewMat, mat4x4 projMat)
    {
        // Update if our transform is changing over time for some reason
        mTrans += mTransDelta;
        mRot += mRotDelta;
        mScale *= mScaleDelta;

        // Apply this node's transformation matrix
        mat4x4 m(1.0f);
        m = glm::scale(m, mScale);

        m = glm::rotate(m, mRot.x, vec3(1.f,0.f,0.f));
        m = glm::rotate(m, mRot.y, vec3(0.f,1.f,0.f));
        m = glm::rotate(m, mRot.z, vec3(0.f,0.f,1.f));
        m = glm::translate(m, mTrans);

        nodeMat = nodeMat * m;

        doRender(nodeMat, viewMat, projMat);
        for( auto& c: mChildren ) c->render(nodeMat, viewMat, projMat);
        //nodeMats.pop_back();
    }

    void Node::doInit() {}
    void Node::doUpload() {}
    void Node::doRender(mat4x4 nodeMat, mat4x4 viewMat, mat4x4 projMat) {}
} }
