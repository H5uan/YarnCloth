#include "GraphicsContext.h"

#include "Platform/OpenGL/OpenGLContext.h"
#include "Platform/Windows/WindowsWindow.h"

Ref<GraphicsContext> GraphicsContext::Create(void* window) {
    return CreateRef<OpenGLContext>(static_cast<GLFWwindow *>(window));
}
