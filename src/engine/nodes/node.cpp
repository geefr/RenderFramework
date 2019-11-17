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

        m *= mUserModelMat;

        m = glm::translate(m, mTrans);

        m = rotate(m, mRot[0], vec3(1.f,0.f,0.f));
        m = rotate(m, mRot[1], vec3(0.f,1.f,0.f));
        m = rotate(m, mRot[2], vec3(0.f,0.f,1.f));

        m = glm::scale(m, mScale);
        
        return m;
    }

    void Node::userModelMatrix(mat4x4 mat) { mUserModelMat = mat; }

    vec3 Node::modelVecToWorldVec( vec3 v )
    {
        // Just dealing with a direction here
        // so don't care about scale/translate
        mat4x4 m(1.0);

        m *= mUserModelMat;
        m = rotate(m, mRot[0], - vec3(1.f,0.f,0.f));
        m = rotate(m, mRot[1], - vec3(0.f,1.f,0.f));
        m = rotate(m, mRot[2], - vec3(0.f,0.f,1.f));
        
        return vec4(v, 1.0f) * m;
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
        // Apply this node's transformation matrix
        nodeMat = nodeMat * matrix();
        
        doRender(nodeMat, viewMat, projMat);
        for( auto& c: mChildren ) c->render(nodeMat, viewMat, projMat);
    }

    void Node::update(double deltaT)
    {
      doUpdate(deltaT);
      for( auto& c : mChildren ) c->update(deltaT);
    }

    void Node::doUpdate(double deltaT)
    {
      // Update if our transform is changing over time for some reason
      mTrans += mTransDelta * (float)deltaT;
      mRot += mRotDelta * (float)deltaT;
      mScale += mScaleDelta * (float)deltaT;
    }

    void Node::doInit() {}
    void Node::doUpload() {}
    void Node::doRender(mat4x4 nodeMat, mat4x4 viewMat, mat4x4 projMat) {}

    std::shared_ptr<colliders::Collider> Node::collider() { return {}; };

} }
