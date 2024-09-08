#pragma once

#include <glm/glm.hpp>

namespace GLCore::Core::Camera {
    class Camera {
    public:
        Camera() = default;

        explicit Camera(const glm::mat4&projection): mProjection(projection) {
        }

        virtual ~Camera() = default;

        [[nodiscard]] const glm::mat4& GetProjection() const { return mProjection; }

    protected:
        glm::mat4 mProjection = glm::mat4(1.0f);
    };
}
