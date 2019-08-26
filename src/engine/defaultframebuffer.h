#ifndef DEFAULTFRAMEBUFFER_H
#define DEFAULTFRAMEBUFFER_H

#include "framebuffer.h"

/**
 * A framebuffer/render target for Engine
 */
namespace renderframework
{
  class DefaultFrameBuffer : public FrameBuffer
  {
  public:
    DefaultFrameBuffer();
    virtual ~DefaultFrameBuffer();

    /// Set the render target to this framebuffer
    virtual void activate() const override final;
    virtual void resolve() const override final;
  };
};

#endif