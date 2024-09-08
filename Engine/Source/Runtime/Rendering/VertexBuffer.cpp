#include "VertexBuffer.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLVertexBuffer.h"


    Ref<VertexBuffer> VertexBuffer::Create(uint32_t size, VertexBufferUsage usage) {
        return CreateRef<OpenGLVertexBuffer>(size, usage);
    }

    Ref<VertexBuffer> VertexBuffer::Create(void* vertices, uint32_t size, VertexBufferUsage usage) {
        return CreateRef<OpenGLVertexBuffer>(vertices, size, usage);
    }

