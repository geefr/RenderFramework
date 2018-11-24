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

    void Node::render()
    {
        doRender();
        for( auto& c: mChildren ) c->render();
    }

    void Node::doInit() {}
    void Node::doUpload() {}
    void Node::doRender() {}
} }
