
#include "framebufferrendertotexture.h"
#include <stdexcept>

namespace renderframework {
  
  FrameBufferRenderToTexture::FrameBufferRenderToTexture(uint32_t width, uint32_t height)
    : mWidth(width), mHeight(height)
  {
    glGenFramebuffers(1, &mFramebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, mFramebuffer);

    // defines for mFramebuffer, tracked in FBO
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    
    glDisable(GL_STENCIL_TEST);

    glGenTextures(1, &mTexture);
    glBindTexture(GL_TEXTURE_2D, mTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
    /*
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, mTexture);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA8, mWidth, mHeight, true);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, mTexture, 0);
    */

    glGenRenderbuffers(1, &mDepthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, mDepthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, mWidth, mHeight);
    //glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH_COMPONENT, mWidth, mHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mDepthBuffer);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, mTexture, 0);

    /*
    glGenFramebuffers(1, &mResolveFramebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, mResolveFramebuffer);

    glGenTextures(1, &mResolveTexture);
    glBindTexture(GL_TEXTURE_2D, mResolveTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mResolveTexture, 0);

    glGenRenderbuffers(1, &mResolveDepthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, mResolveDepthBuffer);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH_COMPONENT, mWidth, mHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mResolveDepthBuffer);
    */

    //GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
    //glDrawBuffers(1, drawBuffers);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) throw std::runtime_error("Failed to initialise framebuffer");
  }

  FrameBufferRenderToTexture::~FrameBufferRenderToTexture()
  {
    //glDeleteRenderbuffers(1, &mResolveDepthBuffer);
    glDeleteRenderbuffers(1, &mDepthBuffer);
    //glDeleteTextures(1, &mResolveTexture);
    glDeleteTextures(1, &mTexture);
    //glDeleteFramebuffers(1, &mResolveFramebuffer);
    glDeleteFramebuffers(1, &mFramebuffer);
  }

  void FrameBufferRenderToTexture::activate() const
  {
    glBindFramebuffer(GL_FRAMEBUFFER, mFramebuffer);


    glViewport(0, 0, mWidth, mHeight);
  }

  void FrameBufferRenderToTexture::resolve() const
  {
    /*
    GLint oDrawFBO = 0, oReadFBO = 0;
    GLboolean oMultiSample = false;
    glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &oDrawFBO);
    glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &oReadFBO);
    glGetBooleanv(GL_MULTISAMPLE, &oMultiSample);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_MULTISAMPLE);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, mFramebuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mResolveFramebuffer);
    glBlitFramebuffer( 0, 0, mWidth, mHeight, 0, 0, mWidth, mHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, oReadFBO);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, oDrawFBO);

    if(oMultiSample) glEnable(GL_MULTISAMPLE);
    */
  }

  //GLuint FrameBufferRenderToTexture::textureID() const { return mResolveTexture; }
  GLuint FrameBufferRenderToTexture::textureID() const { return mTexture; }
  float FrameBufferRenderToTexture::width() const { return mWidth; }
  float FrameBufferRenderToTexture::height() const { return mHeight; }

}
