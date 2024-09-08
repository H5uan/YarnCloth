#pragma once

#include <cstdint>

#include "Rendering/IndexBuffer.h"


class OpenGLIndexBuffer : public IndexBuffer {
public:
    OpenGLIndexBuffer(uint32_t* indices, uint32_t count);

    OpenGLIndexBuffer(uint32_t count);

    virtual ~OpenGLIndexBuffer();

    virtual void Bind() const override;

    virtual void Unbind() const override;

    virtual void SetData(const void* data, uint32_t count) override;

    [[nodiscard]] virtual uint32_t GetCount() const { return mCount; }

private:
    uint32_t mRendererID;
    uint32_t mCount;
};
