#pragma once

#include "Rendering/VertexArray.h"
#include "Rendering/VertexBuffer.h"
#include "Rendering/IndexBuffer.h"
#include "Rendering/Shader.h"
#include "Rendering/Material.h"

#define MAX_BONE_INFLUENCE 4
/***

class Mesh;

struct StaticVertex {
    glm::vec3 Pos;
    glm::vec3 Normal;
    glm::vec2 TexCoord;
    glm::vec3 Tangent;
    glm::vec3 Bitangent;

    int EntityID;
};

struct SkinnedVertex {
    glm::vec3 Pos;
    glm::vec3 Normal;
    glm::vec2 TexCoord;
    glm::vec3 Tangent;
    glm::vec3 Bitangent;

    int EntityID;
};

class SubMesh {
public:
    SubMesh(const std::vector<StaticVertex>&vertices, std::vector<uint32_t> indices);


    SubMesh(const std::vector<StaticVertex>&vertices, std::vector<uint32_t> indices,
            const std::vector<MaterialTexture>&textures, uint32_t materialIndex = 0);

    void Draw(const glm::mat4&transform, const glm::vec3&cameraPos, const Ref<Shader>&shader, int entityID,
              Mesh* model);

    void Draw();

private:
    void SetupMesh(int entityID);

public:
    uint32_t mMaterialIndex;

    std::vector<StaticVertex> mStaticVertices;

private:
    std::vector<MaterialTexture> mTextures;

    std::vector<uint32_t> mIndices;

    Ref<VertexArray> mVertexArray;
    Ref<VertexBuffer> mVB;
    Ref<IndexBuffer> mIB;

    int mEntityID = -1;
};
***/