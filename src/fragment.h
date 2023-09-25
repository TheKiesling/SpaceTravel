#pragma once
#include <glm/glm.hpp>
#include "color.h"

struct Fragment {

    glm::vec2 position; 
    Color color;
    float z;  
    float intensity;

};