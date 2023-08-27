#pragma once
#include "vertex3.h"
#include <vector>
#include "uniforms.h"

Vertex3 vertexShader(const Vertex3 vertex, const Uniforms& uniforms) {
    glm::vec4 transformedVertex = uniforms.viewport * uniforms.projection * uniforms.view * uniforms.model * glm::vec4(vertex.x, vertex.y, vertex.z, 1.0f);
    return Vertex3(glm::vec3(transformedVertex/transformedVertex.w));
}