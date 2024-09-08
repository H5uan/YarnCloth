#include "Mesh/SubMesh.h"
#include "Rendering/RenderingCommand.h"
#include "Library/TextureLibrary.h"
#include "Mesh/Mesh.h"
//#include "GLCore/Resource/ModeManager/ModeManager.h"

/***
SubMesh::SubMesh(const std::vector<StaticVertex>&vertices, const std::vector<uint32_t> indices)
    : mStaticVertices(vertices), mIndices(indices) {
    mVertexArray = VertexArray::Create();

    mVB = VertexBuffer::Create(sizeof(StaticVertex) * vertices.size());
    mVB->SetLayout({
        {ShaderDataType::Float3, "a_Pos"},
        {ShaderDataType::Float3, "a_Normal"},
        {ShaderDataType::Float2, "a_TexCoord"},
        {ShaderDataType::Float3, "a_Tangent"},
        {ShaderDataType::Float3, "a_Bitangent"},
        {ShaderDataType::Int, "a_EntityID"},
    });

    mVertexArray->AddVertexBuffer(mVB);

    mIB = IndexBuffer::Create(indices.size());

    mVertexArray->SetIndexBuffer(mIB);
}

SubMesh::SubMesh(const std::vector<StaticVertex>&vertices, const std::vector<uint32_t> indices,
                 const std::vector<MaterialTexture>&textures, uint32_t materialIndex)
    : mStaticVertices(vertices), mIndices(indices), mTextures(textures), mMaterialIndex(materialIndex) {
    mVertexArray = VertexArray::Create();

    mVB = VertexBuffer::Create(sizeof(StaticVertex) * vertices.size());
    mVB->SetLayout({
        {ShaderDataType::Float3, "a_Pos"},
        {ShaderDataType::Float3, "a_Normal"},
        {ShaderDataType::Float2, "a_TexCoord"},
        {ShaderDataType::Float3, "a_Tangent"},
        {ShaderDataType::Float3, "a_Bitangent"},
        {ShaderDataType::Int, "a_EntityID"},
    });

    mVertexArray->AddVertexBuffer(mVB);

    mIB = IndexBuffer::Create(indices.size());

    mVertexArray->SetIndexBuffer(mIB);
}

void SubMesh::Draw(const glm::mat4&transform, const glm::vec3&cameraPos, const Ref<Shader>&shader, int entityID,
                   Mesh* model) {
    SetupMesh(entityID);

    shader->Bind();


    shader->SetMat4("model", transform);
    shader->SetFloat3("camPos", cameraPos);


    Library<CubeMapTexture>::GetInstance().Get("BlackCubeMap")->Bind(0);
    Library<CubeMapTexture>::GetInstance().Get("BlackCubeMap")->Bind(1);
    Library<Texture2D>::GetInstance().Get("BlackTexture")->Bind(2);


    if (model->mMaterial[mMaterialIndex]->bUseAlbedoMap)
        model->mMaterial[mMaterialIndex]->mAlbedoMap->Bind(3);
    else
        model->mMaterial[mMaterialIndex]->albedoRGBA->Bind(3);

    if (model->mMaterial[mMaterialIndex]->bUseNormalMap)
        model->mMaterial[mMaterialIndex]->mNormalMap->Bind(4);
    else
        Library<Texture2D>::GetInstance().GetWhiteTexture()->Bind(4);

    if (model->mMaterial[mMaterialIndex]->bUseMetallicMap)
        model->mMaterial[mMaterialIndex]->mMetallicMap->Bind(5);
    else
        model->mMaterial[mMaterialIndex]->metallicRGBA->Bind(5);

    if (model->mMaterial[mMaterialIndex]->bUseRoughnessMap)
        model->mMaterial[mMaterialIndex]->mRoughnessMap->Bind(6);
    else
        model->mMaterial[mMaterialIndex]->roughnessRGBA->Bind(6);

    if (model->mMaterial[mMaterialIndex]->bUseAoMap)
        model->mMaterial[mMaterialIndex]->mAoMap->Bind(7);
    else
        Library<Texture2D>::GetInstance().GetWhiteTexture()->Bind(7);

    shader->SetInt("irradianceMap", 0);
    shader->SetInt("prefilterMap", 1);
    shader->SetInt("brdfLUT", 2);
    shader->SetInt("albedoMap", 3);
    shader->SetInt("normalMap", 4);
    shader->SetInt("metallicMap", 5);
    shader->SetInt("roughnessMap", 6);
    shader->SetInt("aoMap", 7);

    RenderCommand::DrawIndexed(mVertexArray, mIB->GetCount());
}

void SubMesh::Draw() {
    //SetupMesh(EntityID);
    static bool bInit = true;
    if (bInit) {
        bInit = false;

        mVertexArray->Bind();

        mVB->SetData(mStaticVertices.data(), sizeof(StaticVertex) * mStaticVertices.size());
        mIB->SetData(mIndices.data(), mIndices.size());

        mVertexArray->Unbind();
    }
    mVertexArray->Bind();
    RenderCommand::DrawIndexed(mVertexArray, mIB->GetCount());
    mVertexArray->Unbind();
}

void SubMesh::SetupMesh(int entityID) {
    if (mEntityID == -1) {
        mEntityID = entityID;
        mVertexArray->Bind();


        for (auto&mStaticVertice: mStaticVertices) {
            mStaticVertice.EntityID = entityID;
        }

        mVB->SetData(mStaticVertices.data(), sizeof(StaticVertex) * mStaticVertices.size());


        mIB->SetData(mIndices.data(), mIndices.size());

        mVertexArray->Unbind();
    }
}
***/