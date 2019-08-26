#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "common.h"

/**
 * A framebuffer/render target for Engine
 */
namespace renderframework
{
  class FrameBuffer
  {
    public:
      FrameBuffer();
      virtual ~FrameBuffer();

      /// Set the render target to this framebuffer, and prepare for drawing
      virtual void activate() const = 0;
      /// Perform post-draw actions, such as resolving a MSAA buffer to a normal texture
      virtual void resolve() const = 0;

      // TODO: These should be members on base class, very much a hack right now
      virtual float width() const;
      virtual float height() const;
  };

  inline FrameBuffer::FrameBuffer() {}
  inline FrameBuffer::~FrameBuffer() {}
  inline float FrameBuffer::width() const { return 0; }
  inline float FrameBuffer::height() const { return 0; }
};

#endif