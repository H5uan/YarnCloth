#pragma once

#include "Rendering/VertexBuffer.h"

class OpenGLVertexBuffer : public VertexBuffer {
public:
    OpenGLVertexBuffer(uint32_t size, VertexBufferUsage usage = VertexBufferUsage::Dynamic);

    OpenGLVertexBuffer(void* vertices, uint32_t size, VertexBufferUsage usage = VertexBufferUsage::Static);

    virtual ~OpenGLVertexBuffer();

    void Bind() const override;

    void Unbind() const override;

    void SetData(const void* data, uint32_t size) override;

    [[nodiscard]] const BufferLayout& GetLayout() const override { return mLayout; }
    void SetLayout(const BufferLayout&layout) override { mLayout = layout; }

private:
    uint32_t mRendererID;
    VertexBufferUsage mUsage;
    BufferLayout mLayout;
};
