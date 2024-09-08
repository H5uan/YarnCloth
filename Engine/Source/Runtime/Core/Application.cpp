#include "glpch.h"
#include "Application.h"

#include "Log.h"

#include "Input.h"

#include <glfw/glfw3.h>

namespace GLCore {
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

    Application::Application(const std::string&name, uint32_t width, uint32_t height) {
        if (!s_Instance) {
            Log::Init();
        }

        GLCORE_ASSERT(!s_Instance, "Application already exists!");
        s_Instance = this;

        const GLCore::WindowProps props(name, width, height);
        m_Window = std::unique_ptr<Window>(Window::Create(props));
        m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);
    }

    void Application::PushLayer(Layer* layer) {
        m_LayerStack.PushLayer(layer);
    }

    void Application::PushOverlay(Layer* layer) {
        m_LayerStack.PushOverlay(layer);
    }

    void Application::OnEvent(Event&e) {
        EventDispatcher dispatcher(e);
        // check if event is window closing event
        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

        // handle the event
        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
            (*--it)->OnEvent(e);
            if (e.Handled)
                break;
        }
    }

    void Application::Run() {
        // app main loop
        while (m_IsRunning) {
            const auto time = static_cast<float>(glfwGetTime());
            const auto timestep = Timestep(time - m_LastFrameTime);
            m_LastFrameTime = time;

            for (Layer* layer: m_LayerStack)
                layer->OnUpdate(timestep);

            m_ImGuiLayer->Begin();
            for (Layer* layer: m_LayerStack)
                layer->OnImGuiRender();
            m_ImGuiLayer->End();

            m_Window->OnUpdate();
        }
    }

    bool Application::OnWindowClose(WindowCloseEvent&e) {
        m_IsRunning = false;
        return true;
    }
}
