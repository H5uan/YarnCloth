#include "glpch.h"
#include "WindowsInput.h"

#include "Core/Application.h"
#include <GLFW/glfw3.h>

namespace GLCore {
    Input* Input::s_Instance = new WindowsInput();

    bool WindowsInput::IsKeyPressedImpl(int keycode) {
        const auto window = static_cast<GLFWwindow *>(Application::Get().GetWindow().GetNativeWindow());
        const int state = glfwGetKey(window, keycode);
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool WindowsInput::IsMouseButtonPressedImpl(int button) {
        const auto window = static_cast<GLFWwindow *>(Application::Get().GetWindow().GetNativeWindow());
        const int state = glfwGetMouseButton(window, button);
        return state == GLFW_PRESS;
    }

    bool WindowsInput::IsMouseButtonReleasedImpl(int button) {
        const auto window = static_cast<GLFWwindow *>(Application::Get().GetWindow().GetNativeWindow());
        const int state = glfwGetMouseButton(window, button);
        return state == GLFW_RELEASE;
    }


    std::pair<float, float> WindowsInput::GetMousePositionImpl() {
        const auto window = static_cast<GLFWwindow *>(Application::Get().GetWindow().GetNativeWindow());
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        return {static_cast<float>(xpos), static_cast<float>(ypos)};
    }

    float WindowsInput::GetMouseXImpl() {
        auto [x, y] = GetMousePositionImpl();
        return x;
    }

    float WindowsInput::GetMouseYImpl() {
        auto [x, y] = GetMousePositionImpl();
        return y;
    }
}
