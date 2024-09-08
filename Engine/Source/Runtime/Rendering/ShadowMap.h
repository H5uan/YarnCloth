#pragma once

#include <glad/glad.h>

#include "Platform/OpenGL/OpenGLFramebuffer.h"
#include "Platform/OpenGL/NativeOpenGLShader.h"


#include <glm/glm.hpp>

#include "Texture/FrameBuffer.h"


class ShadowMap {
public:
    ShadowMap(const uint32_t&resolution = 1024);

    ~ShadowMap() = default;

    [[nodiscard]] Texture2DPtr GetTexture() const { return m_Framebuffer->GetDepthAttachment(); };
    [[nodiscard]] FramebufferPtr GetFramebuffer() const { return m_Framebuffer; };

    void Begin(const glm::mat4&lightViewMatrix, const glm::mat4&lightProjMatrix,
               const float&shadowMapThickness = -1.0f);

    void Clear();

    void End() const;

private:
    FramebufferPtr m_Framebuffer;
    Ref<NativeOpenGLShader> m_Shader;

    float m_Thickness = 0.1f;

    GLint m_CurrViewport[4] = {0, 0, 1280, 720};
};
