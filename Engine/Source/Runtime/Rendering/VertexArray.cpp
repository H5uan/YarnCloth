#include "VertexArray.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"


Ref<VertexArray> VertexArray::Create() {
    return CreateRef<OpenGLVertexArray>();
}
