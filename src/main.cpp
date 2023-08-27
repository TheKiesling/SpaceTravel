#include <SDL2/SDL.h>
#include <iostream>
#include <glm/glm.hpp>
#include <vector>
#include "framebuffer.h"
#include "uniforms.h"
#include "shaders.h"
#include <glm/gtc/matrix_transform.hpp>
#include "loadOBJ.h"

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
Color currentColor;
Framebuffer framebuffer;

std::vector<Vertex3> transformVertexArray(const std::vector<glm::vec3>& vertexArray, const Uniforms& uniforms) {
    std::vector<Vertex3> transformedVertexArray;
    for (const glm::vec3& vertex : vertexArray) {
        transformedVertexArray.push_back(vertexShader(Vertex3(vertex), uniforms));
    }
    return transformedVertexArray;
}

glm::mat4 createModelMatrix(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale) {
    glm::mat4 modelMatrix(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    modelMatrix = glm::scale(modelMatrix, scale);
    return modelMatrix;
}

glm::mat4 createViewMatrix(const glm::vec3& positionCamera, const glm::vec3& centerCamera){
    glm::mat4 viewMatrix(1.0f);
    viewMatrix = glm::lookAt(positionCamera, centerCamera, glm::vec3(0.0f, 1.0f, 0.0f));
    return viewMatrix;
}

glm::mat4 createProjectionMatrix(){
    float fov = 90.0f;
    float aspectRatio = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;
    float near = 0.1f;
    float far = 100.0f;
    glm::mat4 projectionMatrix(1.0f);
    projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, near, far);
    return projectionMatrix;
}

glm::mat4 createViewportMatrix() {
    glm::mat4 viewport = glm::mat4(1.0f);

    // Scale
    viewport = glm::scale(viewport, glm::vec3(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f, 0.5f));

    // Translate
    viewport = glm::translate(viewport, glm::vec3(1.0f, 1.0f, 0.5f));

    return viewport;
}

void render(SDL_Renderer* renderer) {
    std::vector<glm::vec3> modelVertices;
    std::vector<Face> modelFaces;
    bool success = loadOBJ("../src/lab3.obj", modelVertices, modelFaces);

    if (!success) {
        SDL_Log("Error al cargar el modelo OBJ.");
        return;
    }

    std::vector<glm::vec3> vertexArray = setupVertexArray(modelVertices, modelFaces);

    std::vector<Vertex3> transformedVertexArray = transformVertexArray(vertexArray, Uniforms{
        createModelMatrix(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f)),
        createViewMatrix(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f)),
        createProjectionMatrix(),
        createViewportMatrix()
    });

    for (size_t i = 0; i < transformedVertexArray.size(); i += 3) {
        const Vertex3 A(transformedVertexArray[i]);
        const Vertex3 B(transformedVertexArray[i + 1]);
        const Vertex3 C(transformedVertexArray[i + 2]);

        framebuffer.triangle(A, B, C, renderer);
    }

    framebuffer.renderBuffer(renderer);
}

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window* window = SDL_CreateWindow("Cuadrado", 50, 50, WINDOW_WIDTH, WINDOW_HEIGHT, 0);

    SDL_Renderer* renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED
    );

    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        // Limpiar la ventana
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        render(renderer);

        // Actualizar la ventana
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
