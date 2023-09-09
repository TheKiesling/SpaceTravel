#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "line.h"
#include "fragment.h"


std::vector<Fragment> triangle(const glm::vec3& A, const glm::vec3& B, const glm::vec3& C) {
    std::vector<Fragment> fragments;

    // Rasterize the edges using the line function
    std::vector<Fragment> edgeAB = line(A, B);
    std::vector<Fragment> edgeBC = line(B, C);
    std::vector<Fragment> edgeCA = line(C, A);

    // Combine the fragments from each edge
    fragments.insert(fragments.end(), edgeAB.begin(), edgeAB.end());
    fragments.insert(fragments.end(), edgeBC.begin(), edgeBC.end());
    fragments.insert(fragments.end(), edgeCA.begin(), edgeCA.end());

    return fragments;
}