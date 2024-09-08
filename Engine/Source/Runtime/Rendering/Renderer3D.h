#pragma once

#include <GLCore/Rendering/Camera/EditorCamera.h>
#include "Texture.h"

class Renderer3D
{
public:
    static void Init();
    static void Shutdown();

    static void DrawModel(const glm::mat4& transform, const glm::vec3& cameraPos, MeshComponent& MeshComponent, int EntityID);

    static void BeginScene(const Camera& camera, const glm::mat4& transform);
    static void BeginScene(const EditorCamera& camera);
    static void EndScene();
public:
    // shadow pass
    static Ref<class Framebuffer> lightFBO;
};