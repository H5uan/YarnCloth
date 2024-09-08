#pragma once

#include "Core/Layer.h"

#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

namespace GLCore {
    class ImGuiLayer : public Layer {
    public:
        ImGuiLayer();

        ~ImGuiLayer() override = default;

        void OnAttach() override;

        void OnDetach() override;

        void Begin();

        void End();

        void OnEvent(Event&event) override;


    private:
        float m_Time = 0.0f;
        bool m_BlockEvents = true;
    };
}
