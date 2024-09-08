#pragma once

#include "Core.h"
#include "Timestep.h"
#include "../Events/Event.h"

namespace GLCore {
    // Base class for layers. Layers can be different parts shown on the app.
    class Layer {
    public:
        explicit Layer(std::string name = "Layer");

        virtual ~Layer() = default;

        virtual void OnAttach() =0;

        virtual void OnDetach() =0;

        virtual void OnUpdate(Timestep ts) {
        }

        virtual void OnImGuiRender() {
        }

        virtual void OnEvent(Event&event) =0;

        [[nodiscard]] inline const std::string& GetName() const { return m_DebugName; }

    protected:
        std::string m_DebugName;
    };
}
