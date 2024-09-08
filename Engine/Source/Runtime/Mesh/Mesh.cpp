#include "Resource/AssetManager.h"
#include "Mesh/Mesh.h"
#include "Mesh/SubMesh.h"

#include <regex>

#include "Core/Log.h"
/***

void Mesh::Draw(const glm::mat4&transform, const glm::vec3&cameraPos, int entityID) {
    for (unsigned int i = 0; i < mSubMeshes.size(); ++i)
        mSubMeshes[i].Draw(transform, cameraPos, mMaterial[0]->GetShader(), entityID, this);
}

void Mesh::Draw(const glm::mat4&transform, const glm::vec3&cameraPos, Ref<Shader> shader, int entityID) {
    for (unsigned int i = 0; i < mSubMeshes.size(); ++i)
        mSubMeshes[i].Draw(transform, cameraPos, shader, entityID, this);
}

void Mesh::Draw() {
    for (unsigned int i = 0; i < mSubMeshes.size(); ++i)
        mSubMeshes[i].Draw();
}

void Mesh::LoadModel(const std::string&path) {
    mMaterial.resize(200);

    Assimp::Importer importer;
    std::string standardPath = std::regex_replace(path, std::regex("\\\\"), "/");
    std::string standardFullPath =
            std::regex_replace(AssetManager::GetFullPath(path).string(), std::regex("\\\\"), "/");
    const aiScene* scene = importer.ReadFile(standardFullPath,
                                             aiProcess_Triangulate | aiProcess_GenSmoothNormals |
                                             aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        LOG_ERROR("ERROR::ASSIMP::{0}", importer.GetErrorString());
        return;
    }

    mDirectory = standardPath.substr(0, standardPath.find_last_of('/'));

    uint32_t subMeshIndex = 0;

    ProcessNode(scene->mRootNode, scene, subMeshIndex);

    mMaterial.resize(subMeshIndex);
}

void Mesh::ProcessNode(aiNode* node, const aiScene* scene, uint32_t&subMeshIndex) {
    for (uint32_t i = 0; i < node->mNumMeshes; ++i) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];


        mSubMeshes.push_back(ProcessMesh<StaticVertex>(mesh, scene, subMeshIndex));

        subMeshIndex++;
    }

    for (uint32_t i = 0; i < node->mNumChildren; ++i) {
        ProcessNode(node->mChildren[i], scene, subMeshIndex);
    }
}

template<typename Vertex>
SubMesh Mesh::ProcessMesh(aiMesh* mesh, const aiScene* scene, uint32_t&subMeshIndex) {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    std::vector<MaterialTexture> textures;

    for (uint32_t i = 0; i < mesh->mNumVertices; ++i) {
        Vertex vertex;

        //pos
        glm::vec3 vector;
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Pos = vector;

        //normal
        if (mesh->HasNormals()) {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
        }

        //tex coord
        if (mesh->mTextureCoords[0]) {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoord = vec;
        }
        else
            vertex.TexCoord = glm::vec2(0.0f, 0.0f);

        if (mesh->HasTangentsAndBitangents()) {
            // tangent
            vector.x = mesh->mTangents[i].x;
            vector.y = mesh->mTangents[i].y;
            vector.z = mesh->mTangents[i].z;
            vertex.Tangent = vector;
            // bitangent
            vector.x = mesh->mBitangents[i].x;
            vector.y = mesh->mBitangents[i].y;
            vector.z = mesh->mBitangents[i].z;
            vertex.Bitangent = vector;
        }
        else {
            vertex.Tangent = glm::vec3{0.0f};
            vertex.Bitangent = glm::vec3{0.0f};
        }

        vertex.EntityID = -1;

        vertices.push_back(vertex);
    }

    for (uint32_t i = 0; i < mesh->mNumFaces; ++i) {
        aiFace face = mesh->mFaces[i];
        for (uint32_t j = 0; j < face.mNumIndices; ++j) {
            indices.push_back(face.mIndices[j]);
        }
    }

    // process materials
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

    mMaterial[subMeshIndex] = CreateRef<Material>();

    const auto&loadTexture = [&](aiTextureType type) {
        auto maps = loadMaterialTextures(material, type, subMeshIndex);
        if (maps) textures.insert(textures.end(), maps.value().begin(), maps.value().end());
    };

    for (uint32_t type = aiTextureType_NONE; type < aiTextureType_AMBIENT_OCCLUSION; type++) {
        loadTexture(static_cast<aiTextureType>(type));
    }

    return SubMesh(vertices, indices, textures, subMeshIndex);
}

std::optional<std::vector<MaterialTexture>> Mesh::loadMaterialTextures(aiMaterial* mat, aiTextureType type,
                                                                       uint32_t&subMeshIndex) {
    std::vector<MaterialTexture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);

        bool skip = false;
        for (auto&mTexture: mMaterial[subMeshIndex]->mTextures) {
            if (std::strcmp(mTexture.path.data(), str.C_Str()) == 0) {
                textures.push_back(mTexture);
                skip = true;
                break;
            }
        }
        if (!skip) {
            MaterialTexture texture;

            std::string filename = std::string(str.C_Str());
            filename = mDirectory + '/' + filename;
            try {
                texture.texture2d = Texture2D::Create(filename);
            }
            catch (...) {
                LOG_WARN("Load Texture failed!");
                texture.texture2d = Texture2D::Create(AssetManager::GetFullPath("Assets/Textures/DefaultTexture.png"));
            }

            switch (type) {
                case aiTextureType_DIFFUSE:
                case aiTextureType_BASE_COLOR:
                    texture.type = TextureType::Albedo;
                    mMaterial[subMeshIndex]->mAlbedoMap = texture.texture2d;
                    mMaterial[subMeshIndex]->bUseAlbedoMap = true;
                    break;
                case aiTextureType_HEIGHT:
                    texture.type = TextureType::Height;
                    break;
                case aiTextureType_AMBIENT:
                case aiTextureType_AMBIENT_OCCLUSION:
                    texture.type = TextureType::AmbientOcclusion;
                    mMaterial[subMeshIndex]->mAoMap = texture.texture2d;
                    mMaterial[subMeshIndex]->bUseAoMap = true;
                    break;
                case aiTextureType_NORMALS:
                case aiTextureType_NORMAL_CAMERA:
                    texture.type = TextureType::Normal;
                    mMaterial[subMeshIndex]->mNormalMap = texture.texture2d;
                    mMaterial[subMeshIndex]->bUseNormalMap = true;
                    break;
                case aiTextureType_SPECULAR:
                case aiTextureType_METALNESS:
                    texture.type = TextureType::Metalness;
                    mMaterial[subMeshIndex]->mMetallicMap = texture.texture2d;
                    mMaterial[subMeshIndex]->bUseMetallicMap = true;
                    break;
                case aiTextureType_DIFFUSE_ROUGHNESS:
                    texture.type = TextureType::Roughness;
                    mMaterial[subMeshIndex]->mRoughnessMap = texture.texture2d;
                    mMaterial[subMeshIndex]->bUseRoughnessMap = true;
                    break;
                case aiTextureType_EMISSIVE:
                    texture.type = TextureType::Emission;
                    break;
            }
            texture.path = str.C_Str();
            textures.push_back(texture);
            mMaterial[subMeshIndex]->mTextures.push_back(texture);
        }
    }

    if (textures.empty()) return {};
    return textures;
}
***/