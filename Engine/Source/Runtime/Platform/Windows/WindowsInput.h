#pragma once

#include "Core/Input.h"

namespace GLCore {
    class WindowsInput final : public Input {
    protected:
        bool IsKeyPressedImpl(int keycode) override;

        bool IsMouseButtonPressedImpl(int button) override;

        bool IsMouseButtonReleasedImpl(int button) override;

        std::pair<float, float> GetMousePositionImpl() override;

        float GetMouseXImpl() override;

        float GetMouseYImpl() override;
    };
}
