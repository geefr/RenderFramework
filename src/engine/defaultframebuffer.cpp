
#include "common.h"

#include "defaultframebuffer.h"


namespace renderframework {

DefaultFrameBuffer::DefaultFrameBuffer() {}
DefaultFrameBuffer::~DefaultFrameBuffer() {}
void DefaultFrameBuffer::activate() const { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

}