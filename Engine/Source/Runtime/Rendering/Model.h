#pragma once


#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "mesh.h"
#include <string>

#include <map>
#include <vector>

#include "Platform/OpenGL/NativeOpenGLShader.h"
using namespace std;


class Model {
public:
    vector<TextureInfo> textures_loaded;
    vector<OldMesh> meshes;
    string directory;
    bool gammaCorrection = false;

    Model() = default;

    explicit Model(string const&path, const bool gamma = false) : gammaCorrection(gamma) {
        LoadModel(path);
    }

    void Draw(NativeOpenGLShader&shader, const bool draw = false, const bool useTessellation =false, int outerLevel = 0, int innerLevel =0) const {
        for (auto&mesh: meshes)
            mesh.Draw(shader, draw, useTessellation, outerLevel, innerLevel);
    }

private:
    void LoadModel(string const&path);

    vector<TextureInfo> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);


    void ProcessNode(const aiNode* node, const aiScene* scene);

    OldMesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
};

unsigned int TextureFromFile(const char* path, const string&directory, bool gamma = false);
