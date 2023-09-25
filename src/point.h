#pragma once
#include <SDL2/SDL.h>
#include "fragment.h"

const int WINDOW_WIDTH = 1500;
const int WINDOW_HEIGHT = 800;

std::array<double, WINDOW_WIDTH * WINDOW_HEIGHT> zBuffer;

void clearZBuffer() {
    std::fill(zBuffer.begin(), zBuffer.end(), 99999.0f);
}

void point(const Fragment& fragment, SDL_Renderer* renderer) { 
    if (fragment.position.x >= 0 && fragment.position.x < WINDOW_WIDTH && fragment.position.y >= 0 && fragment.position.y < WINDOW_HEIGHT) {
        if (fragment.z < zBuffer[fragment.position.x + fragment.position.y * WINDOW_WIDTH]) {
            SDL_SetRenderDrawColor(renderer, fragment.color.r, fragment.color.g, fragment.color.b, 255);
            SDL_RenderDrawPoint(renderer, fragment.position.x, fragment.position.y);
            zBuffer[fragment.position.x + fragment.position.y * WINDOW_WIDTH] = fragment.z;
        }
    }
}