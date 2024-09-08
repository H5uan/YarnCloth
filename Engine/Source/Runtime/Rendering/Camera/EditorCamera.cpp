#include "EditorCamera.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "Core/Input.h"
#include "Core/KeyCodes.h"
#include "Core/MouseButtonCodes.h"

namespace GLCore::Core::Camera {
    EditorCamera::EditorCamera(float fov, float aspectRatio, float nearClip, float farClip)
        : mFOV(fov), mAspectRatio(aspectRatio), mNearClip(nearClip), mFarClip(farClip),
          Camera(glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip)) {
        UpdateView();
    }

    void EditorCamera::SetCenter(const glm::vec3&center) {
        mFocalPoint = center;
        mDistance = 10.0f;
        UpdateView();
    }

    void EditorCamera::UpdateProjection() {
        mAspectRatio = mViewportWidth / mViewportHeight;
        mProjection = glm::perspective(glm::radians(mFOV), mAspectRatio, mNearClip, mFarClip);
    }

    void EditorCamera::UpdateView() {
        mPosition = CalculatePosition();

        glm::quat orientation = GetOrientation();
        mViewMatrix = glm::translate(glm::mat4(1.0f), mPosition) * glm::toMat4(orientation); // Model & view
        mViewMatrix = glm::inverse(mViewMatrix);
    }

    std::pair<float, float> EditorCamera::PanSpeed() const {
        float x = std::min(mViewportWidth / 1000.0f, 2.4f); // max = 2.4f
        float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

        float y = std::min(mViewportHeight / 1000.0f, 2.4f); // max = 2.4f
        float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

        return {xFactor, yFactor};
    }

    float EditorCamera::RotationSpeed() {
        return 0.8f;
    }

    float EditorCamera::ZoomSpeed() const {
        float distance = mDistance * 0.2f;
        distance = std::max(distance, 0.0f);
        float speed = distance * distance;
        speed = std::min(speed, 100.0f); // max speed = 100
        return speed;
    }

    void EditorCamera::OnUpdate(Timestep ts) {
        if (Input::IsKeyPressed(static_cast<int>(KeyCode::LeftAlt))) {
            const glm::vec2&mouse{Input::GetMouseX(), Input::GetMouseY()};
            glm::vec2 delta = (mouse - mInitialMousePosition) * 0.003f;
            mInitialMousePosition = mouse;

            if (Input::IsMouseButtonPressed(static_cast<int>(MouseButton::Middle)))
                MousePan(delta);
            else if (Input::IsMouseButtonPressed(static_cast<int>(MouseButton::Left)))
                MouseRotate(delta);
            else if (Input::IsMouseButtonPressed(static_cast<int>(MouseButton::Right)))
                MouseZoom(delta.y);
        }
        else if (Input::IsMouseButtonPressed(static_cast<int>(MouseButton::Right))) {
            glm::vec2 deltaMove = {0.0f, 0.0f};

            if (Input::IsKeyPressed(static_cast<int>(KeyCode::A)))
                mFocalPoint -= GetRightDirection() * mCameraSpeed * 0.05f;
            if (Input::IsKeyPressed(static_cast<int>(KeyCode::D)))
                mFocalPoint += GetRightDirection() * mCameraSpeed * 0.05f;
            if (Input::IsKeyPressed(static_cast<int>(KeyCode::W)))
                mFocalPoint += GetForwardDirection() * mCameraSpeed * 0.05f;
            if (Input::IsKeyPressed(static_cast<int>(KeyCode::S)))
                mFocalPoint -= GetForwardDirection() * mCameraSpeed * 0.05f;
            if (Input::IsKeyPressed(static_cast<int>(KeyCode::Q)))
                mFocalPoint -= glm::vec3(0.0f, 1.0f, 0.0f) * mCameraSpeed * 0.05f;
            if (Input::IsKeyPressed(static_cast<int>(KeyCode::E)))
                mFocalPoint += glm::vec3(0.0f, 1.0f, 0.0f) * mCameraSpeed * 0.05f;

            const glm::vec2&mouse{Input::GetMouseX(), Input::GetMouseY()};
            glm::vec2 deltaRotate = (mouse - mInitialMousePosition) * 0.003f;
            if (mInit)
                deltaRotate = {0.0f, 0.0f};
            mInitialMousePosition = mouse;

            if (Input::IsMouseButtonPressed(static_cast<int>(MouseButton::Right))) {
                mInit = false;
                MouseRotate(deltaRotate);
            }
        }
        else if (Input::IsMouseButtonReleased(static_cast<int>(MouseButton::Right))) {
            mInit = true;
        }

        UpdateView();
    }

    void EditorCamera::OnEvent(Event&e) {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<MouseScrolledEvent>(GLCORE_BIND_EVENT_FN(EditorCamera::OnMouseScroll));
    }

    bool EditorCamera::OnMouseScroll(MouseScrolledEvent&e) {
        float delta = e.GetYOffset() * 0.1f;
        MouseZoom(delta);
        UpdateView();
        return false;
    }

    void EditorCamera::MousePan(const glm::vec2&delta) {
        auto [xSpeed, ySpeed] = PanSpeed();
        mFocalPoint += -GetRightDirection() * delta.x * xSpeed * mDistance;
        mFocalPoint += GetUpDirection() * delta.y * ySpeed * mDistance;
    }

    void EditorCamera::MouseRotate(const glm::vec2&delta) {
        float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
        mYaw += yawSign * delta.x * RotationSpeed();
        mPitch += delta.y * RotationSpeed();

        // clamp into limits
        mYaw = fmod(mYaw, 2.0f * glm::pi<float>());
        if (mYaw < 0) mYaw += 2.0f * glm::pi<float>();

        mPitch = glm::clamp(mPitch, -glm::pi<float>() / 2, glm::pi<float>() / 2);
    }

    void EditorCamera::MouseZoom(float delta) {
        mDistance -= delta * ZoomSpeed();
        if (mDistance < 1.0f) {
            mFocalPoint += GetForwardDirection();
            mDistance = 1.0f;
        }
    }

    glm::vec3 EditorCamera::GetUpDirection() const {
        return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
    }

    glm::vec3 EditorCamera::GetRightDirection() const {
        return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
    }

    glm::vec3 EditorCamera::GetForwardDirection() const {
        return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
    }

    glm::vec3 EditorCamera::CalculatePosition() const {
        return mFocalPoint - GetForwardDirection() * mDistance;
    }

    glm::quat EditorCamera::GetOrientation() const {
        return glm::quat(glm::vec3(-mPitch, -mYaw, 0.0f));
    }
}
