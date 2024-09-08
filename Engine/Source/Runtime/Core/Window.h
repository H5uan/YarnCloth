#pragma once

#include <utility>

#include "glpch.h"

#include "Core/Core.h"
#include "Events/Event.h"

namespace GLCore {
    // Discriptor for Window
    struct WindowProps {
        std::string Title;
        uint32_t Width;
        uint32_t Height;

        explicit WindowProps(std::string title = "OpenGL Sandbox",
                             const uint32_t width = 1280,
                             const uint32_t height = 720)
            : Title(std::move(title)), Width(width), Height(height) {
        }
    };

    class Window {
    public:
        using EventCallbackFn = std::function<void(Event&)>;

        virtual ~Window() = default;

        virtual void OnUpdate() = 0;

        [[nodiscard]] virtual uint32_t GetWidth() const = 0;

        [[nodiscard]] virtual uint32_t GetHeight() const = 0;

        // Window attributes
        // Bind events with the Event System
        virtual void SetEventCallback(const EventCallbackFn&callback) = 0;

        virtual void SetVSync(bool enabled) = 0;

        [[nodiscard]] virtual bool IsVSync() const = 0;

        [[nodiscard]] virtual void* GetNativeWindow() const = 0;

        static Window* Create(const WindowProps&props = WindowProps());
    };
}
