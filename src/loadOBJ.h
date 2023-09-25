#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "face.h"
#include <glm/glm.hpp>
#include <SDL2/SDL.h>

bool loadOBJ(const std::string& path, 
             std::vector<glm::vec3>& out_vertices, 
             std::vector<glm::vec2>& out_textureCoords, 
             std::vector<glm::vec3>& out_normals, 
             std::vector<Face>& out_faces) {
    std::ifstream file(path);

    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file " << path << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string type;
        iss >> type;

        if (type == "v") {  // Vertex
            glm::vec3 vertex;
            iss >> vertex.x >> vertex.y >> vertex.z;
            out_vertices.push_back(vertex);
        } else if (type == "vt") {  // Texture Coordinate
            glm::vec3 texCoord;
            iss >> texCoord.x >> texCoord.y;
            out_textureCoords.push_back(texCoord);
        } else if (type == "vn") {  // Normal
            glm::vec3 normal;
            iss >> normal.x >> normal.y >> normal.z;
            out_normals.push_back(normal);
        } else if (type == "f") {  // Face
            Face face;
            int vertexIndex, texCoordIndex, normalIndex;
            while (iss >> vertexIndex) {
                std::array<int, 3> vertexData;
                vertexData[0] = vertexIndex - 1;  // Adjust to 0-based indexing

                // Check for texture and normal data
                if (iss.peek() == '/') {
                    iss.ignore();  // Skip the '/'
                    if (iss.peek() != '/') {
                        iss >> texCoordIndex;  // Texture coordinates
                        vertexData[1] = texCoordIndex - 1;  // Adjust to 0-based indexing
                    }
                    if (iss.peek() == '/') {
                        iss.ignore();  // Skip the second '/'
                        iss >> normalIndex;  // Normal coordinates
                        vertexData[2] = normalIndex - 1;  // Adjust to 0-based indexing
                    }
                }
                face.vertexIndices.push_back(vertexData);
            }
            out_faces.push_back(face);
        }
    }

    file.close();
    return true;
}
