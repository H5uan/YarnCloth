#pragma once

#include <glm/glm.hpp>

#include <filesystem>

#include "Core/Log.h"
#include "Platform/OpenGL/OpenGLIndexBuffer.h"
#include "Platform/OpenGL/OpenGLVertexBuffer.h"

namespace fs = std::filesystem;

namespace fs = std::filesystem;


struct BCCHeader {
    char sign[3];
    unsigned char byteCount;
    char curveType[2],
            dimensions,
            upDimension;
    uint64_t curveCount,
            totalControlPointCount;
    char fileInfo[40];
};

void readCurveData(std::ifstream&file, std::vector<std::vector<glm::vec3>>&fibers, int nbCP) {
    std::vector<glm::vec3> curveData(nbCP);
    for (int cpid = 0; cpid < nbCP; ++cpid) {
        file.read(reinterpret_cast<char *>(&curveData[cpid]), sizeof(glm::vec3));
    }
    fibers.emplace_back(std::move(curveData));
}

void readBCC(const std::string&filename, std::vector<std::vector<glm::vec3>>&closedFibersCP,
             std::vector<std::vector<glm::vec3>>&openFibersCP) {
    closedFibersCP.clear();
    openFibersCP.clear();

    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        LOG_ERROR("Impossible to open the file {0} !", filename);
        return;
    }

    BCCHeader header;
    file.read(reinterpret_cast<char *>(&header), sizeof(header));
    if (header.sign[0] != 'B' || header.sign[1] != 'C' || header.sign[2] != 'C' || header.byteCount != 0x44) {
        LOG_ERROR("Invalid BCC format!");
        return;
    }
    if (header.curveType[0] != 'C' || header.curveType[1] != '0') {
        LOG_ERROR("Invalid Curve type or parametrisation!");
        return;
    }
    if (header.dimensions != 3) {
        LOG_ERROR("Invalid number of dimensions!");
        return;
    }

    for (uint64_t id = 0; id < header.curveCount; ++id) {
        int nbCP;
        file.read(reinterpret_cast<char *>(&nbCP), sizeof(nbCP));
        if (nbCP < 0) {
            readCurveData(file, closedFibersCP, -nbCP);
        }
        else {
            readCurveData(file, openFibersCP, nbCP);
        }
    }

    LOG_INFO("Successfully loaded {} open curves and {} closed curves", openFibersCP.size(), closedFibersCP.size());
}


void LoadBCCFile(const std::string&filePath, std::vector<glm::vec3>&controlPoints, std::vector<uint32_t>&indices) {
    std::vector<std::vector<glm::vec3>> closedFibersCP;
    std::vector<std::vector<glm::vec3>> openFibersCP;
    readBCC(filePath, closedFibersCP, openFibersCP);

    controlPoints.clear();
    indices.clear();

    // Merge all the curves into a single vector to draw all of them in a single drawcall
    // This need to be replaced by the proper loading of the fiber data
    for (const auto&fiber: closedFibersCP) {
        for (const auto&cPoints: fiber)
            controlPoints.push_back(cPoints);
        controlPoints.push_back(fiber.front());
    }
    for (const auto&fiber: openFibersCP)
        for (const auto&cPoints: fiber)
            controlPoints.push_back(cPoints);

    uint32_t vertexCount = controlPoints.size();

    for (size_t i = 0; i < vertexCount - 3; i++) {
        indices.push_back(i);
        indices.push_back(i + 1);
        indices.push_back(i + 2);
        indices.push_back(i + 3);
    }
}


Ref<OpenGLVertexArray> LoadBCCToOpenGL(std::vector<glm::vec3>&controlPoints,
                                       std::vector<uint32_t>&indices) {
    // Send the fibers data to OpenGL
    BufferLayout layout = {
        {ShaderDataType::Float3, "Position"}
    };
    auto vertexBuffer = std::make_shared<OpenGLVertexBuffer>(
        controlPoints.data(),
        controlPoints.size() * sizeof(float) * 3
    );
    vertexBuffer->SetLayout(layout);
    auto indexBuffer = std::make_shared<OpenGLIndexBuffer>(indices.data(), indices.size());

    auto vertexArray = std::make_shared<OpenGLVertexArray>();
    vertexArray->Bind();
    vertexArray->AddVertexBuffer(vertexBuffer);
    vertexArray->SetIndexBuffer(indexBuffer);
    vertexArray->Unbind();
    return vertexArray;
}


/***
std::vector<fs::path> ListBCCFiles(const fs::path&directory) {
    std::vector<fs::path> result;

    fs::directory_iterator start(directory), end;

    std::copy_if(start, end, std::back_inserter(result),
                 [](const fs::directory_entry&entry) {
                     return entry.path().extension() == ".bcc";
                 });

    return result;
}
***/
