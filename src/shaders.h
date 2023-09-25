#pragma once
#include <vector>
#include "uniforms.h"
#include "fragment.h"
#include "color.h"
#include "vertex.h"

Vertex vertexShader(const Vertex& vertex, const Uniforms& uniforms) {
    glm::vec4 transformedVertex = uniforms.viewport * uniforms.projection * uniforms.view * uniforms.model * glm::vec4(vertex.position, 1.0f);
    glm::vec3 screenVertex = transformedVertex/transformedVertex.w;
    glm::vec3 normal = glm::normalize(glm::mat3(uniforms.model) * vertex.normal);
    return Vertex{screenVertex, vertex.color, normal, vertex.textureCoord};
}

Fragment fragmentShader(const Fragment& fragment) {
    Color color = fragment.color * fragment.intensity;
    Fragment newFragment = Fragment{fragment.position, color, fragment.z, fragment.intensity};
    return newFragment;
}