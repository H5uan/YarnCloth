#pragma once

#include "Texture2D.h"
#include <memory>

#include "Core/Base.h"

class Framebuffer;
using FramebufferPtr = Ref<Framebuffer>;

class Framebuffer {
public:
    Framebuffer(const uint32_t&width, const uint32_t&height);

    ~Framebuffer();

    [[nodiscard]] GLuint GetId() const { return m_ID; }

    void Bind();

    void Unbind();

    [[nodiscard]] uint32_t GetWidth() const { return m_Width; }
    [[nodiscard]] uint32_t GetHeight() const { return m_Height; }

    void Resize(const uint32_t&width, const uint32_t&height);

    void AddColorAttachment(const Texture2DPtr&attachment);

    void SetDepthAttachment(const Texture2DPtr&attachment, const bool&depthStencil = false);

    [[nodiscard]] Texture2DPtr GetColorAttachment(uint32_t index) const;

    [[nodiscard]] Texture2DPtr GetDepthAttachment() const;

    void UpdateBuffers();

    void Blit(const GLuint&destId) const;

    void Blit(const FramebufferPtr&destination) const;

    static FramebufferPtr Create(const uint32_t&width, const uint32_t&height);

private:
    GLuint m_ID;

    uint32_t m_Width, m_Height;

    std::vector<Texture2DPtr> m_ColorAttachments;
    std::vector<GLenum> m_DrawBuffers;
    Texture2DPtr m_DepthAttachment;
};
