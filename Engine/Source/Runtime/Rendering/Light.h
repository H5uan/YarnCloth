#pragma once

#include "Core/Timestep.h"
#include "Events/Event.h"
#include "Events/MouseEvent.h"

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

class Light {
public:
    Light() = default;

    glm::vec3 ambient = {0.5f, 0.05f, 0.05f};
    glm::vec3 diffuse = {0.8f, 0.8f, 0.8f};
    glm::vec3 specular = {1.0f, 1.0f, 1.0f};

protected:
    float constant = 1.0f;
    float linear = 0.05f;
    float quadratic = 0.01f;
};

class DirectionalLight {
public:
    DirectionalLight();

    DirectionalLight(const glm::vec3&color, const float&intensity = 1.0f);

    DirectionalLight(const glm::vec3&direction,
                     const glm::vec3&color,
                     const float&intensity = 1.0f);

    glm::vec3 GetDirection() const { return m_direction; }

    void SetDirection(const glm::vec3&direction);

    void SetProjSize(const float&size);

    void SetClipPlanes(const float&nearClip, const float&farClip);

    glm::vec3 GetPower() const { return m_color * m_intensity; }
    glm::mat4 GetViewMatrix() const { return m_viewMatrix; }
    glm::mat4 GetProjectionMatrix() const { return m_projectionMatrix; }

private:
    void UpdateView();

    void UpdateProjection();

    float m_intensity = 1.0;
    glm::vec3 m_color = {1.0f, 1.0f, 1.0f};

    float m_nearClip = 0.1f;
    float m_farClip = 50.0f;
    float m_projSize = 30.0f;

    glm::vec3 m_direction = {0.0f, 0.0f, 1.0f};
    glm::mat4 m_viewMatrix;
    glm::mat4 m_projectionMatrix;
};


class SpotLight : public Light {
public:
    SpotLight() = default;

    SpotLight(float fov, float aspectRatio, float nearClip, float farClip);

    void OnUpdate(GLCore::Timestep ts);

    void OnEvent(GLCore::Event&e);

    [[nodiscard]] float GetDistance() const { return mDistance; }
    void SetDistance(float distance) { mDistance = distance; }

    void SetViewportSize(float width, float height) {
        mViewportWidth = width;
        mViewportHeight = height;
        UpdateProjection();
    }

    std::pair<float, float> GetViewportSize() {
        return {
            mViewportWidth, mViewportHeight
        };
    }

    [[nodiscard]] const glm::mat4& GetViewMatrix() const { return mViewMatrix; }
    [[nodiscard]] glm::mat4 GetViewProjection() const { return mProjection * mViewMatrix; }

    [[nodiscard]] glm::vec3 GetUpDirection() const;

    [[nodiscard]] glm::vec3 GetRightDirection() const;

    [[nodiscard]] glm::vec3 GetForwardDirection() const;

    [[nodiscard]] const glm::vec3& GetPosition() const { return mPosition; }
    [[nodiscard]] const glm::vec3& GetCenter() const { return mFocalPoint; }


    [[nodiscard]] glm::quat GetOrientation() const;

    [[nodiscard]] float GetPitch() const { return mPitch; }
    [[nodiscard]] float GetYaw() const { return mYaw; }

    [[nodiscard]] float GetNearPlane() const { return mNearClip; }
    [[nodiscard]] float GetFarPlane() const { return mFarClip; }

    [[nodiscard]] float GetFOV() const { return mFOV; }
    [[nodiscard]] float GetAspect() const { return mAspectRatio; }

    void SetCenter(const glm::vec3&center);

private:
    void UpdateProjection();

    void UpdateView();


    void MouseRotate(const glm::vec2&delta);


    [[nodiscard]] glm::vec3 CalculatePosition() const;


    [[nodiscard]] static float RotationSpeed();

public:
    float mCameraSpeed = 1.0f;

    float innerCos = cos(glm::radians(50.f));
    float outerCos = cos(glm::radians(52.f));

private:
    float mFOV = 45.0f, mAspectRatio = 1.778f, mNearClip = 0.1f, mFarClip = 1000.0f;

    glm::mat4 mViewMatrix;
    glm::mat4 mProjection;
    glm::vec3 mPosition = {0.0f, 0.0f, 0.0f};
    glm::vec3 mFocalPoint = {0.0f, 0.0f, 0.0f};


    glm::vec2 mInitialMousePosition = {0.0f, 0.0f};

    float mDistance = 3.0f;
    float mPitch = 1.0f, mYaw = 1.0f;

    float mViewportWidth = 2048, mViewportHeight = 2048;

    bool mInit = true;
};
