#include "EditorLayer.h"

#include <stb_image.h>
#include <glad/glad.h>
#include <glfw/glfw3.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>


#include <utility>

#include "Core/Input.h"
#include "Core/KeyCodes.h"
#include "Core/MouseButtonCodes.h"
#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Library/Library.h"
#include "Mesh/Mesh.h"
#include "Rendering/RenderingCommand.h"
#include "Rendering/VertexArray.h"
#include "Rendering/YarnSelfShadow.h"
#include "Rendering/Texture/Texture3D.h"
#include "Resource/BCCReader.h"
#include "Resource/PathResolver.h"

using namespace GLCore;

constexpr float k_leftWindowWidth = 250.0f;
constexpr float k_rightWindowWidth = 330.0f;

EditorLayer::EditorLayer(): m_DirectionalLight(m_LightingSettings.initLightDirection, {0.8f, 0.8f, 0.8f}),
                            m_EditorCamera(
                                45.0f, 16.0f / 9.0f, 0.1f, 1000.0f) {
    fs::path basePath = fs::current_path().parent_path().parent_path().parent_path();
    LOG_INFO("Current project Path: {0}", basePath.string());
    PathResolver::Init(basePath);
}


void EditorLayer::OnAttach() {
    std::string fileRelativePath = "Assets/Model/binary/openwork_trellis_pattern.bcc";
    fs::path fileAbsolutePath = PathResolver::GetInstance().Resolve(fileRelativePath);

    std::vector<glm::vec3> fiberVertices;
    std::vector<uint32_t> fiberIndices;
    LoadBCCFile(fileAbsolutePath.string(), fiberVertices, fiberIndices);
    m_FibersVertexArray = LoadBCCToOpenGL(fiberVertices, fiberIndices);
    m_FibersVertexBuffer = m_FibersVertexArray->GetVertexBuffers()[0];
    m_FibersIndexBuffer = m_FibersVertexArray->GetIndexBuffer();


    m_FiberShader = CreateRef<NativeOpenGLShader>(
        PathResolver::GetInstance().Resolve("Engine\\Shaders\\Fibers.glsl").string()
    );


    m_ShadowMap = std::make_shared<ShadowMap>(4096);

    SelfShadowsSettings selfShadowsSettings = {
        512, 16, static_cast<uint32_t>(m_FiberSettings.plyCount), m_FiberSettings.plyRadius
    };
    m_SelfShadowsTex = SelfShadows::GenerateTexture(selfShadowsSettings);
}

void EditorLayer::OnDetach() {
}

void EditorLayer::OnEvent(Event &event) {
    m_EditorCamera.OnEvent(event);
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<WindowResizeEvent>([&](const WindowResizeEvent &e) {
            m_ViewportSize = {e.GetWidth(), e.GetHeight()};
            RenderCommand::SetViewport(0, 0, e.GetWidth(), e.GetHeight());
            m_EditorCamera.SetViewportSize(e.GetWidth(), e.GetHeight());
            return false;
        }
    );
}

void EditorLayer::OnUpdate(const Timestep ts) {
    m_EditorCamera.OnUpdate(ts);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    RenderCommand::SetClearColor({
        m_RenderingSettings.backgroundColor.r,
        m_RenderingSettings.backgroundColor.g,
        m_RenderingSettings.backgroundColor.b, 1.0f
    });
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glm::mat4 projMat = m_EditorCamera.GetProjection();
    glm::mat4 viewMat = m_EditorCamera.GetViewMatrix();
    glm::mat4 viewInverseMat = glm::inverse(viewMat);
    glm::mat4 modelMat = glm::mat4(1.f);


    if (m_RenderingSettings.useShadowMapping) {
        m_ShadowMap->Begin(m_DirectionalLight.GetViewMatrix(), m_DirectionalLight.GetProjectionMatrix(),
                           m_RenderingSettings.shadowMapThickness);

        m_FibersVertexArray->Bind();
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glDrawElements(GL_PATCHES, m_FibersIndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);
        glDisable(GL_CULL_FACE);
        m_FibersVertexArray->Unbind();

        m_ShadowMap->End();
    } else {
        m_ShadowMap->Clear();
    }


    if (m_RenderingSettings.showFibers) {
        m_FiberShader->Bind();
        m_FiberShader->SetMat4("uProjMatrix", projMat);
        m_FiberShader->SetMat4("uViewMatrix", viewMat);
        m_FiberShader->SetMat4("uModelMatrix", modelMat);

        m_FibersVertexArray->Bind();

        m_FiberShader->SetInt("uPlyCount", m_FiberSettings.plyCount);
        m_FiberShader->SetInt("uTessLineCount", m_FiberSettings.fibersCount);
        m_FiberShader->SetInt("uTessSubdivisionCount", m_FiberSettings.fibersDivisionCount);

        m_FiberShader->SetFloat("R_ply", m_FiberSettings.plyRadius);
        m_FiberShader->SetFloat("Rmin", m_FiberSettings.fiberRadius.x);
        m_FiberShader->SetFloat("Rmax", m_FiberSettings.fiberRadius.y);
        m_FiberShader->SetFloat("theta", m_FiberSettings.fiberRotation);
        m_FiberShader->SetFloat("s", 2.0f); // length of rotation
        m_FiberShader->SetFloat("eN", 1.0f); // ellipse scaling factor along Normal
        m_FiberShader->SetFloat("eB", 1.0f); // ellipse scaling factor along Bitangent

        m_FiberShader->SetFloat("R[0]", 0.20f); // distance from fiber i to ply center
        m_FiberShader->SetFloat("R[1]", 0.25f); // distance from fiber i to ply center
        m_FiberShader->SetFloat("R[2]", 0.30f); // distance from fiber i to ply center
        m_FiberShader->SetFloat("R[3]", 0.35f); // distance from fiber i to ply center

        m_FiberShader->SetFloat3("uLightDirection",
                                 glm::vec3(viewMat * glm::vec4(m_DirectionalLight.GetDirection(), 0.0)));

        // Fragment related uniforms
        m_FiberShader->SetBool("uUseAmbientOcclusion", m_RenderingSettings.useAmbientOcclusion);
        // distance from fiber i to ply center
        m_FiberShader->SetFloat3("fiberColor", m_FiberSettings.fiberColor);

        if (m_RenderingSettings.useShadowMapping) {
            m_FiberShader->SetMat4("uViewToLightMatrix",
                                   m_DirectionalLight.GetProjectionMatrix() * m_DirectionalLight.GetViewMatrix() *
                                   viewInverseMat);
            m_ShadowMap->GetTexture()->Attach(0);
            m_FiberShader->SetInt("uShadowMap", 0);
        } else {
            m_FiberShader->SetMat4("uViewToLightMatrix", glm::mat4(0.0));
            Texture2D::ClearUnit(0);
            m_FiberShader->SetInt("uShadowMap", 0);
        }


        if (m_RenderingSettings.useSelfShadows) {
            m_SelfShadowsTex->Attach(1);
            m_FiberShader->SetInt("uSelfShadowsTexture", 1);
            m_FiberShader->SetFloat("uSelfShadowRotation", m_RenderingSettings.selfShadowRotation);
        } else {
            Texture3D::ClearUnit(1);
            m_FiberShader->SetInt("uSelfShadowsTexture", 1);
        }

        glPatchParameteri(GL_PATCH_VERTICES, 4);

        glDrawElements(GL_PATCHES, m_FibersIndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);
        m_FibersVertexArray->Unbind();
    }
}

void EditorLayer::OnImGuiRender() {
    ImGuiIO &io = ImGui::GetIO();

    ImGui::Begin("Control panel", nullptr); {
        ImGui::PushItemWidth(100.0f);

        if (ImGui::CollapsingHeader("Profiling", ImGuiTreeNodeFlags_DefaultOpen)) {
            indentedLabel("FPS :");
            ImGui::SameLine();
            ImGui::Text("%.1f (%.3fms)", io.Framerate, 1000.0f / io.Framerate);

            ImGui::Spacing();
        }

        if (ImGui::CollapsingHeader("Scenes"), ImGuiTreeNodeFlags_DefaultOpen) {
        }

        if (ImGui::CollapsingHeader("Fiber generation settings", ImGuiTreeNodeFlags_DefaultOpen)) {
            indentedLabel("Ply count :");
            ImGui::SameLine();
            if (ImGui::DragInt("##PlyCountDrag", &m_FiberSettings.plyCount, 0.1f, 0, 10,
                               m_FiberSettings.plyCount > 1 ? "%d ply" : "%d plies")) {
                m_SelfShadowsSettings.plyCount = m_FiberSettings.plyCount;
                m_SelfShadowsTex = SelfShadows::GenerateTexture(m_SelfShadowsSettings);
            }

            indentedLabel("Fibers count :");
            ImGui::SameLine();
            if (ImGui::DragInt("##FibersCountDrag", &m_FiberSettings.fibersCount, 0.1f, m_FiberSettings.plyCount,
                               64,
                               m_FiberSettings.fibersCount > 1 ? "%d fibers" : "%d fiber")) {
                m_FiberSettings.fibersCount = std::max(m_FiberSettings.plyCount, m_FiberSettings.fibersCount);
            }


            indentedLabel("Fibers divisions :");
            ImGui::SameLine();
            if (ImGui::DragInt("##FibersDivisonDrag", &m_FiberSettings.fibersDivisionCount, 0.1f, 2, 64,
                               m_FiberSettings.fibersDivisionCount > 1 ? "%d divisions" : "%d division")) {
                m_FiberSettings.fibersDivisionCount = std::max(2, m_FiberSettings.fibersDivisionCount);
            }

            indentedLabel("Ply radius :");
            ImGui::SameLine();
            ImGui::DragFloat("##PlyRadiusDrag", &m_FiberSettings.plyRadius, 0.01f, 0.0f, 5.0f, "%.2f",
                             ImGuiSliderFlags_Logarithmic);

            indentedLabel("Fibers radius :");
            ImGui::SameLine();
            ImGui::DragFloat2("##FibersRadiusDrag", &m_FiberSettings.fiberRadius.x, 0.01f, 0.0f, 5.0f, "%.2f",
                              ImGuiSliderFlags_Logarithmic);

            indentedLabel("Fibers rotation :");
            ImGui::SameLine();
            ImGui::DragFloat("##FibersRotationDrag", &m_FiberSettings.fiberRotation, 0.01f, -5.0f, 5.0f, "%.2f");
        }

        if (ImGui::CollapsingHeader("Rendering settings", ImGuiTreeNodeFlags_DefaultOpen)) {
            indentedLabel("Show fibers :");
            ImGui::SameLine();
            ImGui::Checkbox("##ShowFibersCB", &m_RenderingSettings.showFibers);

            indentedLabel("Ambient occlusion :");
            ImGui::SameLine();
            ImGui::Checkbox("##UseAmbientOcclusion", &m_RenderingSettings.useAmbientOcclusion);

            indentedLabel("Self Shadows :");
            ImGui::SameLine();
            ImGui::Checkbox("##UseSelfShadows", &m_RenderingSettings.useSelfShadows);

            indentedLabel("Shadow Mapping :");
            ImGui::SameLine();
            ImGui::Checkbox("##UseShadowMapping", &m_RenderingSettings.useShadowMapping);

            ImGui::BeginDisabled(!m_RenderingSettings.useShadowMapping);
            indentedLabel("Shadow Map Thickess :");
            ImGui::SameLine();
            ImGui::DragFloat("##ShadowMapThicknessSlider", &m_RenderingSettings.shadowMapThickness, 0.001f, 0.0f,
                             1.0);
            ImGui::EndDisabled();

            indentedLabel("Background Color :");
            ImGui::SameLine();
            ImGui::PushItemWidth(
                (ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x) -
                ImGui::GetCursorPosX());
            ImGui::ColorEdit3("##BackgroundColorSlider", &m_RenderingSettings.backgroundColor.r,
                              ImGuiColorEditFlags_Float);
            ImGui::PopItemWidth();

            indentedLabel("Fibers Color :");
            ImGui::SameLine();
            ImGui::PushItemWidth(
                (ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x) -
                ImGui::GetCursorPosX());
            ImGui::ColorEdit3("##FiberColorSlider", &m_FiberSettings.fiberColor.r, ImGuiColorEditFlags_Float);
            ImGui::PopItemWidth();

            ImGui::Spacing();
        }

        ImGui::PopItemWidth();
        ImGui::End();
    }
}
