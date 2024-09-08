#include "YarnSelfShadow.h"

#include "Resource/PathResolver.h"

constexpr double PI = 3.14159265358979323846;


FramebufferPtr SelfShadows::s_densityFramebuffer;
FramebufferPtr SelfShadows::s_absorptionFramebuffer;
Ref<NativeOpenGLShader> SelfShadows::s_densityShader;
Ref<NativeOpenGLShader> SelfShadows::s_absorptionShader;

std::shared_ptr<Texture3D> SelfShadows::GenerateTexture(const SelfShadowsSettings&settings) {
    if (!s_densityShader) {
        PathResolver&resolver = PathResolver::GetInstance();
        s_densityShader = CreateRef<NativeOpenGLShader>(
            resolver.Resolve("Engine/Shaders/SelfShadowDensity.glsl").string());
        s_absorptionShader = CreateRef<NativeOpenGLShader>(
            resolver.Resolve("Engine/Shaders/SelfShadowAbsorption.glsl").string());
    }

    GLint restoreViewport[4];
    glGetIntegerv(GL_VIEWPORT, restoreViewport);

    // Density framebuffer
    s_densityFramebuffer = std::make_shared<Framebuffer>(settings.textureSize, settings.textureSize);
    s_densityFramebuffer->Bind();
    s_densityFramebuffer->AddColorAttachment(std::make_shared<Texture2D>(settings.textureSize,
                                                                         settings.textureSize,
                                                                         GL_RGBA8, true));

    // Absorption framebuffer
    s_absorptionFramebuffer = std::make_shared<Framebuffer>(settings.textureSize, settings.textureSize);
    s_absorptionFramebuffer->Bind();
    auto absorptionTexture = std::make_shared<Texture2D>(settings.textureSize,
                                                         settings.textureSize,
                                                         GL_RGBA8, true);
    s_absorptionFramebuffer->AddColorAttachment(absorptionTexture);

    // Dummy vao to render in full screen
    GLuint dummyVAO;
    glGenVertexArrays(1, &dummyVAO);
    glBindVertexArray(dummyVAO);

    // Sampling the ply density between [0, 2*PI/nPlyplyCount]
    float plyAngleStep = 2.0 * PI / static_cast<float>(settings.plyCount) / static_cast<float>(settings.
                             textureCount);
    uint32_t pixelCount = settings.textureSize * settings.textureSize;
    std::vector<uint8_t> texture3DData(pixelCount * settings.textureCount);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    for (uint32_t i = 0; i < settings.textureCount; ++i) {
        // Render density
        s_densityShader->Bind();
        s_densityShader->SetInt("uPlyCount", settings.plyCount);
        s_densityShader->SetFloat("uPlyAngle", plyAngleStep * static_cast<float>(i));
        s_densityShader->SetFloat("uPlyRadius", settings.plyRadius);
        s_densityShader->SetFloat("uFiberRadius", settings.fiberRadius);
        s_densityShader->SetFloat("uDensityE", settings.densityE);
        s_densityShader->SetFloat("uDensityB", settings.densityB);
        s_densityShader->SetFloat("uEN", settings.eN);
        s_densityShader->SetFloat("uEB", settings.eB);

        s_densityFramebuffer->Bind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Render absorption from density
        s_absorptionShader->Bind();
        s_densityFramebuffer->GetColorAttachment(0)->Attach(5);
        s_absorptionShader->SetInt("uDensityTexture", 5);

        s_absorptionShader->SetFloat("uPlyRadius", settings.plyRadius);
        s_absorptionShader->SetFloat("uFiberRadius", settings.fiberRadius);

        s_absorptionFramebuffer->Bind();
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Accumulate 2D "slice" in a vector
        absorptionTexture->Bind();
        absorptionTexture->GetData(GL_RED, GL_UNSIGNED_BYTE, &texture3DData[pixelCount * i]);
    }
    absorptionTexture->Unbind();
    s_absorptionFramebuffer->Unbind();
    glBindVertexArray(0);

    auto result = std::make_shared<Texture3D>(settings.textureSize,
                                              settings.textureSize,
                                              settings.textureCount,
                                              GL_R8, GL_RED, GL_UNSIGNED_BYTE,
                                              texture3DData.data(),
                                              true);

    glViewport(restoreViewport[0],
               restoreViewport[1],
               restoreViewport[2],
               restoreViewport[3]);

    return result;
}
