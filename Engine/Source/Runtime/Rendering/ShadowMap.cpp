#include "ShadowMap.h"

#include "Resource/PathResolver.h"

ShadowMap::ShadowMap(const uint32_t &resolution) {
    PathResolver &resolver = PathResolver::GetInstance();

    auto texture = Texture2D::Create(resolution, resolution, GL_DEPTH_COMPONENT24, true);
    texture->Bind();
    texture->SetWrappingFlags(GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER);
    float defaultColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
    texture->SetFloatParameter(GL_TEXTURE_BORDER_COLOR, defaultColor);
    texture->Unbind();

    m_Framebuffer = Framebuffer::Create(resolution, resolution);
    m_Framebuffer->Bind();
    m_Framebuffer->SetDepthAttachment(texture);
    m_Framebuffer->UpdateBuffers(); // Explicitly set the drawbuffers to None
    m_Framebuffer->Unbind();

    m_Shader = std::make_shared<NativeOpenGLShader>(
        resolver.Resolve("Engine/Shaders/ShadowMap.glsl").string()
    );
}

void ShadowMap::Begin(const glm::mat4 &lightViewMatrix, const glm::mat4 &lightProjMatrix,
                      const float &shadowMapThickness) {
    // Backup viewport dimensions to restore them during End()
    glGetIntegerv(GL_VIEWPORT, m_CurrViewport);
    if (shadowMapThickness > 0.0f)
        m_Thickness = shadowMapThickness;

    m_Shader->Bind();
    m_Shader->SetMat4("uModelMatrix", glm::mat4(1.0f));
    m_Shader->SetMat4("uViewMatrix", lightViewMatrix);
    m_Shader->SetMat4("uProjMatrix", lightProjMatrix);

    m_Shader->SetInt("uTessLineCount", 64); // Rendering the yarn as a single tube
    m_Shader->SetInt("uTessSubdivisionCount", 4);
    m_Shader->SetFloat("uThickness", m_Thickness); // Should have the value of R_ply or a mix of R_ply and Rmin/Rmax

    m_Framebuffer->Bind();
    glClear(GL_DEPTH_BUFFER_BIT);
}

void ShadowMap::End() const {
    m_Framebuffer->Unbind();

    glViewport(m_CurrViewport[0],
               m_CurrViewport[1],
               m_CurrViewport[2],
               m_CurrViewport[3]);
}

void ShadowMap::Clear() {
    glGetIntegerv(GL_VIEWPORT, m_CurrViewport);

    m_Framebuffer->Bind();
    glClear(GL_DEPTH_BUFFER_BIT);
    End();
}
