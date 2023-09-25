#pragma once
#include <vector>
#include <array>
#include <glm/glm.hpp>
#include "vertex.h"

struct Face {
    std::vector<std::array<int, 3>> vertexIndices;
    std::vector<std::array<int, 3>> normalIndices;
    std::vector<std::array<int, 3>> textureIndices;
};

std::vector<Vertex> setupVertexArray(const std::vector<glm::vec3>& vertices,
                                     const std::vector<glm::vec2>& textureCoords,
                                     const std::vector<glm::vec3>& normals,
                                     const std::vector<Face>& faces) {
    std::vector<Vertex> vertexArray;

    // For each face
    for (const auto& face : faces) {
        // For each vertex in the face
        for (const auto& vertexIndices : face.vertexIndices) {
            Vertex vertex;

            // Get the vertex position, texture coordinates, and normal from the input arrays
            // using the indices from the face
            vertex.position = vertices[vertexIndices[0]];

            if (vertexIndices[1] != -1) {
                vertex.textureCoord = textureCoords[vertexIndices[1]];
            }

            if (vertexIndices[2] != -1) {
                vertex.normal = normals[vertexIndices[2]];
            }

            // Add the vertex to the vertex array
            vertexArray.push_back(vertex);
        }
    }

    return vertexArray;
}
