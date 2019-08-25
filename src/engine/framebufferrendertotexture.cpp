
#include "framebufferrendertotexture.h"
#include <stdexcept>

namespace renderframework {
  
  FrameBufferRenderToTexture::FrameBufferRenderToTexture(uint32_t width, uint32_t height)
    : mWidth(width), mHeight(height)
  {
    glGenFramebuffers(1, &mFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, mFBO);

    glGenTextures(1, &mTexture);
    glBindTexture(GL_TEXTURE_2D, mTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glGenRenderbuffers(1, &mDepthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, mDepthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, mWidth, mHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mDepthBuffer);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, mTexture, 0);
    GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, drawBuffers);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) throw std::runtime_error("Failed to initialise framebuffer");
  }

  FrameBufferRenderToTexture::~FrameBufferRenderToTexture()
  {
    glDeleteRenderbuffers(1, &mDepthBuffer);
    glDeleteTextures(1, &mTexture);
    glDeleteFramebuffers(1, &mFBO);
  }

  void FrameBufferRenderToTexture::activate() const
  {
    glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
    glViewport(0,0,mWidth,mHeight);
  }

  GLuint FrameBufferRenderToTexture::textureID() const { return mTexture; }
  float FrameBufferRenderToTexture::width() const { return mWidth; }
  float FrameBufferRenderToTexture::height() const { return mHeight; }

}