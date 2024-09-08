#include "Mesh.h"

#include <utility>


OldMesh::OldMesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<TextureInfo> textures,
                 const glm::vec3 center)
    : m_Vertives(std::move(vertices)),
      m_Indices(std::move(indices)),
      m_Textures(std::move(textures)),
      m_Center(center), VAO(0), VBO(0), EBO(0) {
    SetupOldMesh();
}

void OldMesh::Draw(NativeOpenGLShader&shader, const bool point, const bool useTessellation, int outerLevel, int innerLevel) const {
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    unsigned int heightNr = 1;

    for (unsigned int i = 0; i < m_Textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);

        string number;
        string name = m_Textures[i].type;
        if (name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if (name == "texture_specular")
            number = std::to_string(specularNr++);
        else if (name == "texture_normal")
            number = std::to_string(normalNr++);
        else if (name == "texture_height")
            number = std::to_string(heightNr++);
        glBindTextureUnit(i, m_Textures[i].id);
        glUniform1i(glGetUniformLocation(shader.mRendererID, (name + number).c_str()), i);
    }

    glBindVertexArray(VAO);
    if (useTessellation) {
        glPatchParameteri(GL_PATCH_VERTICES, 3);
        glUniform1i(glGetUniformLocation(shader.mRendererID, "outerLevel"), outerLevel);
        glUniform1i(glGetUniformLocation(shader.mRendererID, "innerLevel"), innerLevel);
        glDrawElements(GL_PATCHES, static_cast<int>(m_Indices.size()), GL_UNSIGNED_INT, nullptr);
    }
    else {
        glDrawElements(point ? GL_POINTS : GL_TRIANGLES, static_cast<int>(m_Indices.size()), GL_UNSIGNED_INT, nullptr);
    }
    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
}

void OldMesh::SetupOldMesh() {
    glCreateVertexArrays(1, &VAO);

    glCreateBuffers(1, &VBO);
    glNamedBufferData(VBO, m_Vertives.size() * sizeof(Vertex), &m_Vertives[0], GL_STATIC_DRAW);

    glCreateBuffers(1, &EBO);
    glNamedBufferData(EBO, m_Indices.size() * sizeof(unsigned int), &m_Indices[0], GL_STATIC_DRAW);

    // link vbo
    glVertexArrayElementBuffer(VAO, EBO);

    // vertex attrib pointer
    // position
    glEnableVertexArrayAttrib(VAO, 0);
    glVertexArrayAttribFormat(VAO, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, Position));
    glVertexArrayAttribBinding(VAO, 0, 0);

    // normal
    glEnableVertexArrayAttrib(VAO, 1);
    glVertexArrayAttribFormat(VAO, 1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, Normal));
    glVertexArrayAttribBinding(VAO, 1, 0);

    // texcoords
    glEnableVertexArrayAttrib(VAO, 2);
    glVertexArrayAttribFormat(VAO, 2, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, TexCoords));
    glVertexArrayAttribBinding(VAO, 2, 0);

    glEnableVertexArrayAttrib(VAO, 3);
    glVertexArrayAttribFormat(VAO, 3, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, Tangent));
    glVertexArrayAttribBinding(VAO, 3, 0);

    glEnableVertexArrayAttrib(VAO, 4);
    glVertexArrayAttribFormat(VAO, 4, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, Bitangent));
    glVertexArrayAttribBinding(VAO, 4, 0);

    // binding vbo
    glVertexArrayVertexBuffer(VAO, 0, VBO, 0, sizeof(Vertex));
}
