#pragma once


#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"

#include <string>
#include <vector>

#include "Platform/OpenGL/NativeOpenGLShader.h"
using namespace std;


struct Vertex {
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoords;
    // tangent
    glm::vec3 Tangent;
    // bitangent
    glm::vec3 Bitangent;
};

struct TextureInfo {
    unsigned int id;
    string type;
    string path;
};


class OldMesh {
public:
    vector<Vertex> m_Vertives;
    vector<unsigned int> m_Indices;
    vector<TextureInfo> m_Textures;
    glm::vec3 m_Center;
    unsigned int VAO;

    OldMesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<TextureInfo> textures, glm::vec3 center);

    void Draw(NativeOpenGLShader&shader, bool point = false, bool useTessellation =false, int outerLevel = 0, int innerLevel = 0) const;

private:
    unsigned int VBO, EBO;

    void SetupOldMesh();
};
