#include "Framebuffer.h"


Framebuffer::Framebuffer(const uint32_t&width, const uint32_t&height) : m_Width(width),
                                                                        m_Height(height) {
    glGenFramebuffers(1, &m_ID);
}

Framebuffer::~Framebuffer() {
    glDeleteFramebuffers(1, &m_ID);
    m_ID = 0;
}

void Framebuffer::Bind() {
    glViewport(0, 0, m_Width, m_Height);
    glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
}

void Framebuffer::Unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::Resize(const uint32_t&width, const uint32_t&height) {
    if (m_Width == width && m_Height == height) {
        return;
    }

    for (auto&attachment: m_ColorAttachments) {
        attachment->Resize(width, height);
    }

    if (m_DepthAttachment) {
        m_DepthAttachment->Resize(width, height);
    }

    m_Width = width;
    m_Height = height;
}

void Framebuffer::AddColorAttachment(const Texture2DPtr&attachment) {
    GLenum drawBuffer = GL_COLOR_ATTACHMENT0 + m_ColorAttachments.size();
    glFramebufferTexture2D(GL_FRAMEBUFFER, drawBuffer, GL_TEXTURE_2D, attachment->GetId(), 0);
    m_ColorAttachments.push_back(attachment);
    m_DrawBuffers.push_back(drawBuffer);

    glDrawBuffers(m_DrawBuffers.size(), m_DrawBuffers.data());
}

void Framebuffer::SetDepthAttachment(const Texture2DPtr&attachment, const bool&depthStencil) {
    if (depthStencil) {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, attachment->GetId(), 0);
    }
    else {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, attachment->GetId(), 0);
    }
    m_DepthAttachment = attachment;
}

void Framebuffer::UpdateBuffers() {
    glDrawBuffers(m_DrawBuffers.size(), m_DrawBuffers.data());
}

Texture2DPtr Framebuffer::GetColorAttachment(const uint32_t index) const {
    if (index > m_ColorAttachments.size()) {
        return nullptr;
    }

    return m_ColorAttachments[index];
}

Texture2DPtr Framebuffer::GetDepthAttachment() const {
    return m_DepthAttachment;
}

void Framebuffer::Blit(const GLuint&destFrameBufferId) const {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, destFrameBufferId);
    glBlitFramebuffer(0, 0, m_Width, m_Height,
                      0, 0, m_Width, m_Height,
                      GL_COLOR_BUFFER_BIT, GL_NEAREST);

    // Restore the current framebuffer as the drawing buffer
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_ID);
}

void Framebuffer::Blit(const FramebufferPtr&destination) const {
    Blit(destination->GetId());
}

FramebufferPtr Framebuffer::Create(const uint32_t&width, const uint32_t&height) {
    return std::make_shared<Framebuffer>(width, height);
}
