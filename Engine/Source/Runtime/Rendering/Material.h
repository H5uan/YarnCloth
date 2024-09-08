/***
#pragma once

#include "Texture.h"
#include "Shader.h"

#include <unordered_map>
#include <utility>

#include "Core/Core.h"

struct EnumClassHash {
    template<typename T>
    std::size_t operator()(T t) const {
        return static_cast<std::size_t>(t);
    }
};

enum class TextureType {
    Albedo = 0,
    Normal,
    Metalness,
    Roughness,
    AmbientOcclusion,
    Specular,
    Height,
    Emission,
};

struct MaterialTexture {
    Ref<Texture2D> texture2d = nullptr;
    TextureType type;
    std::string path;
};

class Material {
public:
    Material() { Initialize(); };
    explicit Material(Ref<Shader> shader) : mShader(std::move(shader)) { Initialize(); };

public:
    void SetShader(const Ref<Shader>&shader) { mShader = shader; }
    [[nodiscard]] Ref<Shader> GetShader() { return mShader; }

    void AddTexture(TextureType type, const Ref<Texture2D>&texture) {
        GLCORE_ASSERT(mTexMap.find(type) == mTexMap.end());
        mTexMap[type] = texture;
    }

    [[nodiscard]] Ref<Texture2D> GetTexture(const TextureType type) { return mTexMap[type]; }

private:
    void Initialize() const;

public:
    std::vector<MaterialTexture> mTextures;

    bool bUseAlbedoMap = false;
    glm::vec4 col = {1.0f, 1.0f, 1.0f, 1.0f}; // 0 ~ 1
    Ref<Texture2D> albedoRGBA = Texture2D::Create(1, 1);
    //Ref<Texture2D> mAlbedoMap = Library<Texture2D>::GetInstance().GetDefaultTexture();
    Ref<Texture2D> mAlbedoMap{};

    bool bUseNormalMap = false;
    //Ref<Texture2D> mNormalMap = Library<Texture2D>::GetInstance().Get("DefaultNormal");
    Ref<Texture2D> mNormalMap{};

    bool bUseMetallicMap = false;
    float metallic = 0.1f;
    Ref<Texture2D> metallicRGBA = Texture2D::Create(1, 1);
    //Ref<Texture2D> mMetallicMap = Library<Texture2D>::GetInstance().Get("DefaultMetallicRoughness");
    Ref<Texture2D> mMetallicMap{};

    bool bUseRoughnessMap = false;
    float roughness = 0.9f;
    Ref<Texture2D> roughnessRGBA = Texture2D::Create(1, 1);
    //Ref<Texture2D> mRoughnessMap = Library<Texture2D>::GetInstance().Get("DefaultMetallicRoughness");
    Ref<Texture2D> mRoughnessMap{};

    bool bUseAoMap = false;
    //Ref<Texture2D> mAoMap = Library<Texture2D>::GetInstance().GetWhiteTexture();
    Ref<Texture2D> mAoMap{};

private:
    Ref<Shader> mShader;
    std::unordered_map<TextureType, Ref<Texture2D>, EnumClassHash> mTexMap;
};
***/