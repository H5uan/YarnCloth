#include "UniformBuffer.h"

#include "Core/Base.h"
#include "Platform/OpenGL/OpenGLUniformBuffer.h"

Ref<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding) {
    return CreateRef<OpenGLUniformBuffer>(size, binding);
}
