#pragma once

#include "Core/Window.h"

#include <GLHeaders.h>

namespace GLCore {
    class WindowsWindow final : public Window {
    public:
        explicit WindowsWindow(const WindowProps&props);

        ~WindowsWindow() override;

        void OnUpdate() override;

        [[nodiscard]] inline uint32_t GetWidth() const override { return m_WindowDiscription.width_; }
        [[nodiscard]] inline uint32_t GetHeight() const override { return m_WindowDiscription.height_; }

        // Window attributes
        inline void SetEventCallback(const EventCallbackFn&callback) override {
            m_WindowDiscription.event_callback = callback;
        }

        void SetVSync(bool enabled) override;

        [[nodiscard]] bool IsVSync() const override;

        [[nodiscard]] inline void* GetNativeWindow() const override { return m_Window; }

    private:
        virtual void Init(const WindowProps&props);

        virtual void Shutdown();

    private:
        GLFWwindow* m_Window{};

        struct WindowDescription {
            std::string title_;
            uint32_t width_, height_;
            bool VSync_;

            EventCallbackFn event_callback;
        };

        WindowDescription m_WindowDiscription;
    };
}
