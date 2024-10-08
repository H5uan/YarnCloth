#pragma once
#include "Platform/Windows/WindowsWindow.h"
#include "Rendering/GraphicsContext.h"

class OpenGLContext : public GraphicsContext
{
public:
    OpenGLContext(GLFWwindow* windowHandle);

    void Init() override;
    void SwapBuffers() override;
private:
    GLFWwindow* m_WindowHandle;
};
