#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "line.h"
#include "fragment.h"
#include "vertex.h"
#include "point.h"

glm::vec3 L = glm::vec3(0.0f, 0.0f, 1.0f);

glm::vec3 barycentricCoordinates(const glm::vec3& P, const glm::vec3& A, const glm::vec3& B, const glm::vec3& C){
    glm::vec3 bary = glm::cross(
        glm::vec3(C.x - A.x, B.x - A.x, A.x - P.x),
        glm::vec3(C.y - A.y, B.y - A.y, A.y - P.y)
    );

    if (abs(bary.z) < 1) {
        return glm::vec3(-1, 1, 1);
    }

    float w = 1.0f - (bary.x + bary.y) / bary.z;
    float v = bary.y / bary.z;
    float u = bary.x / bary.z;

    return glm::vec3(w, v, u);
}

std::vector<Fragment> triangle(const Vertex& a, const Vertex& b, const Vertex& c) {
    std::vector<Fragment> fragments;
    glm::vec3 A = a.position;
    glm::vec3 B = b.position;
    glm::vec3 C = c.position;

    float minX = static_cast<int>(std::ceil(std::min(std::min(A.x, B.x), C.x)));
    float minY = static_cast<int>(std::ceil(std::min(std::min(A.y, B.y), C.y)));
    float maxX = static_cast<int>(std::floor(std::max(std::max(A.x, B.x), C.x)));
    float maxY = static_cast<int>(std::floor(std::max(std::max(A.y, B.y), C.y)));

    // Iterate over each point in the bounding box
    for (int y = minY; y <= maxY; ++y) {
        for (int x = minX; x <= maxX; ++x) {

            if (x < 0 || x >= WINDOW_WIDTH || y < 0 || y >= WINDOW_HEIGHT)
                continue;
            
            glm::vec3 P(x, y, 0.0f);
            auto barycentric = barycentricCoordinates(P, A, B, C);
            float u = barycentric.z;
            float v = barycentric.y;
            float w = barycentric.x;
            float epsilon = 1e-10;

            if (w < epsilon || v < epsilon || u < epsilon)
                continue;
    
            float z = A.z * w + B.z * v + C.z * u;
        
            glm::vec3 normal = glm::normalize(a.normal * w + b.normal * v + c.normal * u);

            float intensity = glm::dot(normal, L);

            if (intensity < 0)
                continue;

            Color color = Color{205, 205, 205};

            glm::vec3 original = a.original * w + b.original * v + c.original * u;

            fragments.push_back(Fragment{glm::vec3(x, y, z), color, z, intensity, original});
        }
    }
    return fragments;
}