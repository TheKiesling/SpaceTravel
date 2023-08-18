#pragma once
#include <vector>
#include "color.h"
#include <algorithm>
#include <SDL2/SDL.h>

const int FRAMEBUFFER_WIDTH = 80;
const int FRAMEBUFFER_HEIGHT = 80;
const int FRAMEBUFFER_SIZE = FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT;
const int WINDOW_WIDTH = 600;
const int WINDOW_HEIGHT = 600;

struct Framebuffer {
    Color framebuffer[FRAMEBUFFER_SIZE];
    Color clearColor;
    Color currentColor;

    Framebuffer()
    : clearColor(Color(0, 0, 0)),
        currentColor(Color(255, 255, 255)) {
            clear();
        }

    int x = 10;
    int y = 10;

    void clear() {
        for (int i = 0; i < FRAMEBUFFER_SIZE; i++) {
            framebuffer[i] = clearColor;
        }
    }

    void point(int x, int y) {
        if (x >= 0 && x < FRAMEBUFFER_WIDTH && y >= 0 && y < FRAMEBUFFER_HEIGHT) {
            framebuffer[y * FRAMEBUFFER_WIDTH + x] = currentColor;
        }
    }

    void renderBuffer(SDL_Renderer* renderer) {
        SDL_Texture* texture = SDL_CreateTexture(
            renderer, 
            SDL_PIXELFORMAT_RGB24,
            SDL_TEXTUREACCESS_STREAMING,
            FRAMEBUFFER_WIDTH, 
            FRAMEBUFFER_HEIGHT
        );

        SDL_UpdateTexture(
            texture, 
            NULL, 
            framebuffer, 
            FRAMEBUFFER_WIDTH * sizeof(Color)
        );

        SDL_RenderCopy(renderer, texture, NULL, NULL);

        SDL_DestroyTexture(texture);
    }

    

    void initialRender(){
        clear();
    }

    void render(SDL_Renderer* renderer) {
        renderBuffer(renderer);
    }
};
