#pragma once
#include "color.h"
#include <SDL2/SDL.h>
#include "vertex3.h"
#include "face.h"

const int WINDOW_WIDTH = 1500;
const int WINDOW_HEIGHT = 800;
const int FRAMEBUFFER_SIZE = WINDOW_WIDTH * WINDOW_HEIGHT;

struct Framebuffer {
    Color framebuffer[FRAMEBUFFER_SIZE];
    Color clearColor;
    Color currentColor;
    
    Framebuffer()
    : clearColor(Color(0, 0, 0)),
        currentColor(Color(255, 255, 255)) {
            clear();
        }

    void clear() {
        for (int i = 0; i < FRAMEBUFFER_SIZE; i++) {
            framebuffer[i] = clearColor;
        }
    }

    void setColor(const Color& color) {
        currentColor = color;
    }

    void point(const Vertex3& vertex, SDL_Renderer* renderer) { 
        int width, height;
        SDL_GetRendererOutputSize(renderer, &width, &height);

        int x = static_cast<int>(vertex.x * static_cast<float>(WINDOW_WIDTH) / static_cast<float>(width));
        int y = static_cast<int>(vertex.y * static_cast<float>(WINDOW_HEIGHT) / static_cast<float>(height));

        if (x >= 0 && x < WINDOW_WIDTH && y >= 0 && y < WINDOW_HEIGHT) {
            framebuffer[y * WINDOW_WIDTH + x] = currentColor;
            SDL_SetRenderDrawColor(renderer, currentColor.r, currentColor.g, currentColor.b, 255);
            SDL_RenderDrawPoint(renderer, x, y);
        }
    }
    

    void line(const Vertex3& start, const Vertex3& end, SDL_Renderer* renderer) {
        int x1 = static_cast<int>(round(start.x));
        int y1 = static_cast<int>(round(start.y));
        int x2 = static_cast<int>(round(end.x));
        int y2 = static_cast<int>(round(end.y));

        int dx = abs(x2 - x1);
        int dy = abs(y2 - y1);
        int sx = (x1 < x2) ? 1 : -1;
        int sy = (y1 < y2) ? 1 : -1;
        int err = dx - dy;

        while (true) {
            Vertex3 pointVertex(glm::vec3(static_cast<float>(x1), static_cast<float>(y1), 0.0f));

            point(pointVertex, renderer);

            if (x1 == x2 && y1 == y2) {
                break; 
            }

            int e2 = 2 * err;

            if (e2 > -dy) {
                err -= dy;
                x1 += sx;
            }

            if (e2 < dx) {
                err += dx;
                y1 += sy;
            }
        }
    }


    void triangle(const Vertex3& A, const Vertex3& B, const Vertex3& C, SDL_Renderer* renderer) {
        line(A, B, renderer);
        line(B, C, renderer);
        line(C, A, renderer);
    }

    void renderBuffer(SDL_Renderer* renderer) {
        SDL_Texture* texture = SDL_CreateTexture(
            renderer, 
            SDL_PIXELFORMAT_RGB24,
            SDL_TEXTUREACCESS_STREAMING,
            WINDOW_WIDTH, 
            WINDOW_HEIGHT
        );

        SDL_UpdateTexture(
            texture, 
            NULL, 
            framebuffer, 
            WINDOW_WIDTH * sizeof(Color)
        );

        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, texture, NULL, NULL);

        SDL_DestroyTexture(texture);
    }

};
