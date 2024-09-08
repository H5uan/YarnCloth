#include "OpenGLContext.h"
/***
OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
    : m_WindowHandle(windowHandle) {
    GLCORE_ASSERT(windowHandle, "Window handle is null!")
}

void OpenGLContext::Init() {
    glfwMakeContextCurrent(m_WindowHandle);
    int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    GLCORE_ASSERT(status, "Failed to initialize Glad!");

    LOG_INFO("OpenGL Info:");
    LOG_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
    LOG_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
    LOG_INFO("  Version: {0}", glGetString(GL_VERSION));

    GLCORE_ASSERT(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 5),
                  "Hazel requires at least OpenGL version 4.5!");
}

void OpenGLContext::SwapBuffers() {
    glfwSwapBuffers(m_WindowHandle);
}
***/
