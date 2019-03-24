#ifndef NODE_H
#define NODE_H

#include "common.h"

#include <memory>
#include <vector>

namespace renderframework { namespace nodes {
    /**
     * Node within the scene graph
     *
     * - A node can contain other nodes
     * - The sub-classes provide leaf nodes, which contain models/things that can be rendered
     *
     */
    class Node
    {
        using Child = std::shared_ptr<Node>;
        using Children = std::vector<Child>;
    public:
        Node();
        virtual ~Node();

        // Child accessorss
        Children& children();

        // Node transformations
        vec3& scale();
        vec3& rotation();
        vec3& translation();

        // Node transformation deltas
        // Will be applied each time render is called
        vec3& scaleDelta();
        vec3& rotationDelta();
        vec3& translationDelta();

        // Model Matrix
        // Recalculates every time it's called
        mat4x4 matrix() const;

        // Convert a vector in model space to world space
        // typically used to work out which way 'forward' is
        // for a model
        vec3 modelVecToWorldVec( vec3 v );

        // Initialise this node and any children
        // All one-time initialisation should be put here
        // so stuff like buffer generation, fetching uniform IDs etc
        void init();
        // Upload this node and any children to the GPU
        // It should be possible to call this multiple times
        // to allow buffer contents/textures etc to change
        void upload();
        // Render this node and any children
        void render(mat4x4 viewMat, mat4x4 projMat);
        void render(mat4x4 nodeMat, mat4x4 viewMat, mat4x4 projMat);
    protected:
        // Children need to implement these
        // Will be called by the public versions of these
        // methods for this node
        // Child classes do not need to worry about their
        // respective sub-nodes, just anything they've added
        // as part of the child class ;)
        virtual void doInit();
        virtual void doUpload();
        virtual void doRender(mat4x4 nodeMat, mat4x4 viewMat, mat4x4 projMat);

    private:
        vec3 mScale = vec3(1.0);
        vec3 mRot = vec3(0.0);
        vec3 mTrans = vec3(0.0);

        vec3 mScaleDelta = vec3(1.0);
        vec3 mRotDelta = vec3(0.0);
        vec3 mTransDelta = vec3(0.0);

        Children mChildren;
    };
} }
#endif // NODE_H
