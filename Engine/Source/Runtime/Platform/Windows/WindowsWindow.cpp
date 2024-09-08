#include "glpch.h"
#include "WindowsWindow.h"

#include "Events/ApplicationEvent.h"
#include "Events/MouseEvent.h"
#include "Events/KeyEvent.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Core/KeyCodes.h"

namespace GLCore {
    static bool glfw_initialized = false;

    static void GLFWErrorCallback(int error, const char* description) {
        LOG_ERROR("GLFW Error ({0}): {1}", error, description);
    }

    Window* Window::Create(const WindowProps&props) {
        return new WindowsWindow(props);
    }

    WindowsWindow::WindowsWindow(const WindowProps&props) {
        Init(props);
    }

    WindowsWindow::~WindowsWindow() {
        Shutdown();
    }

    void WindowsWindow::Init(const WindowProps&props) {
        m_WindowDiscription.title_ = props.Title;
        m_WindowDiscription.width_ = props.Width;
        m_WindowDiscription.height_ = props.Height;

        if (!glfw_initialized) {
            int success = glfwInit();
            GLCORE_ASSERT(success, "Could not intialize GLFW!");
            glfwSetErrorCallback(GLFWErrorCallback);
            glfw_initialized = true;
        }

        m_Window = glfwCreateWindow(static_cast<int>(props.Width), static_cast<int>(props.Height),
                                    m_WindowDiscription.title_.c_str(), nullptr,
                                    nullptr);

        glfwMakeContextCurrent(m_Window);
        int status = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
        GLCORE_ASSERT(status, "Failed to initialize Glad!");

        LOG_INFO("OpenGL Info:");
        LOG_INFO("  Vendor: {0}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
        LOG_INFO("  Renderer: {0}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
        LOG_INFO("  Version: {0}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));

        glfwSetWindowUserPointer(m_Window, &m_WindowDiscription);
        SetVSync(true);

        // Set GLFW callbacks
        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
            WindowDescription&data = *static_cast<WindowDescription *>(glfwGetWindowUserPointer(window));
            data.width_ = width;
            data.height_ = height;

            WindowResizeEvent event(width, height);
            data.event_callback(event);
            // Whenever the window size changes, the viewport is also updated
        });

        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
            const auto&[title_, width_, height_, VSync_, event_callback] = *static_cast<WindowDescription *>(
                glfwGetWindowUserPointer(window));
            WindowCloseEvent event;
            event_callback(event);
        });

        glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
            WindowDescription&data = *static_cast<WindowDescription *>(glfwGetWindowUserPointer(window));

            switch (action) {
                case GLFW_PRESS: {
                    KeyPressedEvent event(key, 0);
                    data.event_callback(event);
                    // press esc to call WindowCloseEvent
                    if (key == static_cast<int>(KeyCode::Escape)) {
                        WindowCloseEvent closeEvent;
                        data.event_callback(closeEvent);
                    }
                    break;
                }
                case GLFW_RELEASE: {
                    KeyReleasedEvent event(key);
                    data.event_callback(event);
                    break;
                }
                case GLFW_REPEAT: {
                    KeyPressedEvent event(key, 1);
                    data.event_callback(event);
                    break;
                }
            }
        });

        glfwSetCharCallback(m_Window, [](GLFWwindow* window, uint32_t keycode) {
            WindowDescription&data = *static_cast<WindowDescription *>(glfwGetWindowUserPointer(window));

            KeyTypedEvent event(keycode);
            data.event_callback(event);
        });

        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {
            WindowDescription&data = *static_cast<WindowDescription *>(glfwGetWindowUserPointer(window));

            switch (action) {
                case GLFW_PRESS: {
                    MouseButtonPressedEvent event(button);
                    data.event_callback(event);
                    break;
                }
                case GLFW_RELEASE: {
                    MouseButtonReleasedEvent event(button);
                    data.event_callback(event);
                    break;
                }
            }
        });

        glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset) {
            WindowDescription&data = *static_cast<WindowDescription *>(glfwGetWindowUserPointer(window));

            MouseScrolledEvent event((float)xOffset, (float)yOffset);
            data.event_callback(event);
        });

        glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos) {
            WindowDescription&data = *static_cast<WindowDescription *>(glfwGetWindowUserPointer(window));

            MouseMovedEvent event(static_cast<float>(xPos), static_cast<float>(yPos));
            data.event_callback(event);
        });
    }

    void WindowsWindow::Shutdown() {
        glfwDestroyWindow(m_Window);
    }

    void WindowsWindow::OnUpdate() {
        glfwPollEvents();
        glfwSwapBuffers(m_Window);
    }

    void WindowsWindow::SetVSync(bool enabled) {
        if (enabled)
            glfwSwapInterval(1);
        else
            glfwSwapInterval(0);

        m_WindowDiscription.VSync_ = enabled;
    }

    bool WindowsWindow::IsVSync() const {
        return m_WindowDiscription.VSync_;
    }
}
