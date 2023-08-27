#pragma once
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <iostream>

struct Vertex3 {
    float x;
    float y;
    float z;

    Vertex3(glm::vec3 vec) : x(vec.x), y(vec.y), z(vec.z) {}
};