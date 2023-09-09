#pragma once
#include <SDL2/SDL.h>
#include "fragment.h"

const int WINDOW_WIDTH = 1500;
const int WINDOW_HEIGHT = 800;

void point(const Fragment& fragment, SDL_Renderer* renderer) { 
    if (fragment.x >= 0 && fragment.x < WINDOW_WIDTH && fragment.y >= 0 && fragment.y < WINDOW_HEIGHT) {
        SDL_SetRenderDrawColor(renderer, fragment.color.r, fragment.color.g, fragment.color.b, 255);
        SDL_RenderDrawPoint(renderer, fragment.x, fragment.y);
    }
}