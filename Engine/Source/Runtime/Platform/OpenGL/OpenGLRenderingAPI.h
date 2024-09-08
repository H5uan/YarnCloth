#pragma once

#include "Rendering/RenderingAPI.h"

class OpenGLRenderingAPI : public RenderingAPI {
public:
    void Init() override;

    void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

    void SetClearColor(const glm::vec4&color) override;

    void Clear() override;

    void DrawIndexed(const Ref<VertexArray>&vertexArray, uint32_t indexCount = 0) override;

    void DrawLines(const Ref<VertexArray>&vertexArray, uint32_t vertexCount) override;

    void SetLineWidth(float width) override;

    void ClearStencil() override;

    void DepthMask(bool maskFlag) override;

    void DepthTest(bool enable) override;

    void Blend(int32_t Bit) override;

    void StencilTest(int32_t Bit) override;

    void BindTexture(int32_t slot, uint32_t textureID) override;

    void Cull(int32_t Bit) override;

    void CullFrontOrBack(bool bFront) override;

    void SetStencilFunc(StencilFunc stencilFunc, int32_t ref, int32_t mask) override;

    void StencilMask(uint32_t mask) override;

    void SetFrontOrBackStencilOp(int32_t FrontOrBack, StencilOp stencilFail, StencilOp depthFail,
                                 StencilOp depthSuccess) override;

    void DepthFunc(DepthComp comp) override;

    int GetDrawFrameBuffer() override;

    void BindFrameBuffer(uint32_t framebufferID) override;
};
