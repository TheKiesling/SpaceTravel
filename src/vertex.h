#pragma once
#include <glm/glm.hpp>
#include "color.h"

struct Vertex {
    glm::vec3 position;
    Color color;
    glm::vec3 normal;
    glm::vec2 textureCoord;
    glm::vec3 original;
};