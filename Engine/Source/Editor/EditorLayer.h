#pragma once

#include <GLCore.h>
#include <GLCoreUtils.h>

#include "Scene.h"
#include "Core/Base.h"
#include "Core/Layer.h"
#include "Rendering/Model.h"
#include "Rendering/Texture/Framebuffer.h"
#include "Rendering/Light.h"
#include "Rendering/Camera/EditorCamera.h"
#include "Platform/OpenGL/NativeOpenGLShader.h"
#include "Platform/OpenGL/OpenGLTexture.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"
#include "Rendering/ShadowMap.h"
#include "Rendering/YarnSelfShadow.h"
#include "Rendering/Texture/Texture3D.h"
#include "Resource/PathResolver.h"


struct FiberSettings {
    int plyCount = 3;
    int fibersCount = 64;
    int fibersDivisionCount = 4;
    float plyRadius = 0.1f;
    glm::vec2 fiberRadius = {0.1f, 0.2f};
    float fiberRotation = 1.0f;
    glm::vec3 fiberColor = glm::vec3(1.f, 0.3f, 0.3f);
};

struct RenderingSettings {
    bool showFibers = true;
    bool showClothMesh = false;

    bool useAmbientOcclusion = true;

    bool useShadowMapping = true;
    bool useSelfShadows = true;

    float shadowMapThickness = 0.15f;
    float selfShadowRotation = 0.0f;

    glm::vec3 backgroundColor = {0.6f, 0.6f, 0.6f};
};

struct LightingSettings {
    glm::vec3 initLightDirection = glm::normalize(glm::vec3(0.5f, -0.5f, -0.5f));
    float lightRotation = 0.0f;
    bool animateLightRotation = false;
};

class EditorLayer final : public GLCore::Layer {
public:
    EditorLayer();

    void OnAttach() override;

    void OnDetach() override;

    void OnEvent(GLCore::Event &event) override;

    void OnUpdate(GLCore::Timestep ts) override;

    void OnImGuiRender() override;

    void SetScenes(const vector<shared_ptr<Scene> > &initializers) {
        m_Scenes = initializers;
    }

    void SwitchScene(unsigned int sceneIndex) {

    }

private:
    void indentedLabel(const std::string &label, const float &indent = 150.0f) {
        ImGui::AlignTextToFramePadding();
        float labelWidth = ImGui::CalcTextSize(label.c_str()).x;
        ImGui::SetCursorPosX(indent - labelWidth);
        ImGui::Text(label.c_str());
    }

    Ref<NativeOpenGLShader> m_FiberShader;


    FiberSettings m_FiberSettings;
    RenderingSettings m_RenderingSettings;
    LightingSettings m_LightingSettings;

    std::shared_ptr<ShadowMap> m_ShadowMap;
    SelfShadowsSettings m_SelfShadowsSettings;
    std::shared_ptr<Texture3D> m_SelfShadowsTex;

    DirectionalLight m_DirectionalLight;

    std::shared_ptr<OpenGLVertexArray> m_FibersVertexArray;
    std::shared_ptr<VertexBuffer> m_FibersVertexBuffer;
    std::shared_ptr<IndexBuffer> m_FibersIndexBuffer;

    glm::vec2 m_ViewportSize = {1280.0f, 720.0f};


    GLCore::Core::Camera::EditorCamera m_EditorCamera;


    float azimuth = 20.0f;
    float elevation = 30.0f;
    float radius = 30.0f;
    float heightScale = 0.05f;
    int outerLevel = 32;
    int innerLevel = 32;
    bool ctrlPressed = false;
    bool altPressed = false;
    bool spacePressed = false;
    glm::vec2 lastMousePos{};
    float m_Sensitivity = 0.03f;
    float m_Reflectivity = 0.5f;

    std::vector<std::shared_ptr<Scene> > m_Scenes;
    unsigned int m_SceneIndex = 0;
    unsigned int m_NextSceneIndex = 0;

};
