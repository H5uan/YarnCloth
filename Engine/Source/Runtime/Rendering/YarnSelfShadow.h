#pragma once

#include "Rendering/Texture/Framebuffer.h"
#include "Rendering/Texture/Texture2D.h"
#include "Rendering/Texture/Texture3D.h"
#include "Platform/OpenGL/NativeOpenGLShader.h"

struct SelfShadowsSettings {
    float textureSize = 512;
    uint32_t textureCount = 16; // slices in 3D texutre

    uint32_t plyCount = 3;
    float plyRadius = 0.1; // P_ply
    float fiberRadius = 0.1; // Rmin
    float densityE = 0.25;
    float densityB = 0.75;
    float eN = 1.0;
    float eB = 1.0;
};

class SelfShadows {
public:
    static std::shared_ptr<Texture3D> GenerateTexture(const SelfShadowsSettings&settings);

private:
    SelfShadows() = delete;

    SelfShadows(const SelfShadows&) = delete;

    static FramebufferPtr s_densityFramebuffer;
    static FramebufferPtr s_absorptionFramebuffer;
    static Ref<NativeOpenGLShader> s_densityShader;
    static Ref<NativeOpenGLShader> s_absorptionShader;
};
