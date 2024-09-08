#pragma once

#include <memory>
#include <string>
#include <cstdint>

#include "Core.h"
#include "Window.h"
#include "LayerStack.h"
#include "../Events/Event.h"
#include "../Events/ApplicationEvent.h"
#include "Timestep.h"
#include "../ImGui/ImGuiLayer.h"

namespace GLCore {
    class Application {
    public:
        explicit Application(const std::string&name = "OpenGL App", uint32_t width = 1280, uint32_t height = 720);

        virtual ~Application() = default;

        virtual void Run();

        void OnEvent(Event&e);

        void PushLayer(Layer* layer);

        void PushOverlay(Layer* layer);

        [[nodiscard]] inline Window& GetWindow() const { return *m_Window; }

        [[nodiscard]] inline static Application& Get() { return *s_Instance; }

    private:
        bool OnWindowClose(WindowCloseEvent&e);

    private:
        std::unique_ptr<Window> m_Window;
        ImGuiLayer* m_ImGuiLayer;
        bool m_IsRunning = true;
        LayerStack m_LayerStack;
        float m_LastFrameTime = 0.0f;

    private:
        inline static Application* s_Instance = nullptr;
    };
}
