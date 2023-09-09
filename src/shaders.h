#pragma once
#include <vector>
#include "uniforms.h"
#include "fragment.h"
#include "color.h"

glm::vec3 vertexShader(const glm::vec3& vertex, const Uniforms& uniforms) {
    glm::vec4 transformedVertex = uniforms.viewport * uniforms.projection * uniforms.view * uniforms.model * glm::vec4(vertex.x, vertex.y, vertex.z, 1.0f);
    return glm::vec3(transformedVertex/transformedVertex.w);
}

Color fragmentShader(const Fragment& fragment) {
    return Color(255, 255, 255);
}