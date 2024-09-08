/***
#include "Library/Library.h"
#include "Resource/AssetManager.h"
#include "Mesh/Mesh.h"
#include "Rendering/Texture.h"
#include "Library/UniformBufferLibrary.h"


template<typename LibType>
Library<LibType>::Library() = default;

template<>
Library<Mesh>::Library() {
    Add("Cube", CreateRef<Mesh>("Assets/Models/cube.obj"));
}

template<>
Library<CubeMapTexture>::Library() {
    std::vector<std::string> Paths{
        "Assets/Textures/Skybox/cubemap_posx.png",
        "Assets/Textures/Skybox/cubemap_negx.png",
        "Assets/Textures/Skybox/cubemap_posy.png",
        "Assets/Textures/Skybox/cubemap_negy.png",
        "Assets/Textures/Skybox/cubemap_posz.png",
        "Assets/Textures/Skybox/cubemap_negz.png",
    };
    Add("SkyBoxTexture", CubeMapTexture::Create(Paths));

    std::vector<std::string> blackPaths{
        "Assets/Textures/black.png",
        "Assets/Textures/black.png",
        "Assets/Textures/black.png",
        "Assets/Textures/black.png",
        "Assets/Textures/black.png",
        "Assets/Textures/black.png",
    };
    Add("BlackCubeMap", CubeMapTexture::Create(blackPaths));
}
***/