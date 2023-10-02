#include <SDL2/SDL.h>
#include <iostream>
#include <glm/glm.hpp>
#include <vector>
#include "uniforms.h"
#include "shaders.h"
#include <glm/gtc/matrix_transform.hpp>
#include "loadOBJ.h"
#include "triangle.h"
#include "camera.h"
#include "point.h"
#include "fragment.h"

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

enum class Model {
    Earth,
    Moon,
    Sun, 
    Venus,
    Saturn,
    Ring
};

glm::mat4 createModelMatrix(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale) {
    glm::mat4 modelMatrix(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    modelMatrix = glm::scale(modelMatrix, scale);
    return modelMatrix;
}

glm::mat4 createViewMatrix(const Camera& camera){
    glm::mat4 viewMatrix(1.0f);
    viewMatrix = glm::lookAt(camera.position, camera.center, camera.up);
    return viewMatrix;
}

glm::mat4 createProjectionMatrix(){
    float fov = 45.0f;
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

std::vector<std::vector<Vertex>> primitiveAssembly(const std::vector<Vertex>& transformedVertexArray) {
    std::vector<std::vector<Vertex>> assembledVertexArray;
    for (int i = 0; i < transformedVertexArray.size(); i += 3) {
        std::vector<Vertex> triangle;
        triangle.push_back(transformedVertexArray[i]);
        triangle.push_back(transformedVertexArray[i + 1]);
        triangle.push_back(transformedVertexArray[i + 2]);
        assembledVertexArray.push_back(triangle);
    }
    return assembledVertexArray;
}

std::vector<Fragment> rasterize(const std::vector<std::vector<Vertex>>& assembledVertexArray) {
    std::vector<Fragment> fragments;
    for (const std::vector<Vertex>& triangleVertex : assembledVertexArray) {
        std::vector<Fragment> triangleFragments = triangle(triangleVertex[0], triangleVertex[1], triangleVertex[2]);
        fragments.insert(fragments.end(), triangleFragments.begin(), triangleFragments.end());
    }
    return fragments;
}

void render(const std::vector<Vertex>& vertexArray, const Uniforms& uniforms, const Model& planet) {
    // Vertex Shader
    std::vector<Vertex> transformedVertexArray;
    for (const Vertex& vertex : vertexArray) {
        transformedVertexArray.push_back(vertexShader(vertex, uniforms));
    }

    // Primitive Assembly
    std::vector<std::vector<Vertex>> assembledVertexArray = primitiveAssembly(transformedVertexArray);

    // Rasterization
    std::vector<Fragment> fragments = rasterize(assembledVertexArray);
    
    // Fragment Shader
    for (Fragment& fragment : fragments) {
        switch (planet) {
            case Model::Earth:
                fragment = earth(fragment);
                break;
            case Model::Moon:
                fragment = moon(fragment);
                break;
            case Model::Sun:
                fragment = sun(fragment);
                break;
            case Model::Venus:
                fragment = venus(fragment);
                break;
            case Model::Saturn:
                fragment = saturn(fragment);
                break;
            case Model::Ring:
                fragment = ring(fragment);
                break;
            default:
                fragment = fragmentShader(fragment);
                break;
        }
        point(fragment, renderer);
    }
}

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_EVERYTHING);

    window = SDL_CreateWindow("Space Travel", 50, 50, WINDOW_WIDTH, WINDOW_HEIGHT, 0);

    renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED
    );

    std::vector<glm::vec3> modelVertices, modelVerticesCircle;
    std::vector<glm::vec2> modelTextureCoords, modelTextureCoordsCircle;
    std::vector<glm::vec3> modelNormals, modelNormalsCircle;
    std::vector<Face> modelFaces, modelFacesCircle;
    bool successSphere = loadOBJ("../models/sphere.obj", modelVertices, modelTextureCoords, modelNormals, modelFaces);
    bool succesCircle = loadOBJ("../models/circle.obj", modelVerticesCircle, modelTextureCoordsCircle, modelNormalsCircle, modelFacesCircle);

    std::vector<Vertex> vertexArraySphere = setupVertexArray(modelVertices, modelTextureCoords, modelNormals, modelFaces);
    std::vector<Vertex> vertexArrayCircle = setupVertexArray(modelVerticesCircle, modelTextureCoordsCircle, modelNormalsCircle, modelFacesCircle);

    Camera camera {
        glm::vec3(0.0f, 0.0f, 3.0f),
        glm::vec3(0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    };

    Uniforms uniformsPlanet{
        createModelMatrix(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f)),
        createViewMatrix(camera),
        createProjectionMatrix(),
        createViewportMatrix()
    };

    Uniforms uniformsMoon{
        createModelMatrix(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.5f)),
        createViewMatrix(camera),
        createProjectionMatrix(),
        createViewportMatrix()
    };

    Uniforms uniformsRing{
        createModelMatrix(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f)),
        createViewMatrix(camera),
        createProjectionMatrix(),
        createViewportMatrix()
    };

    bool running = true;
    SDL_Event event;

    int rotation = 0;

    while (running) {
        clearZBuffer();
        rotation += 1;
        uniformsPlanet.model = createModelMatrix(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, rotation, 0.0f), glm::vec3(1.0f));
        uniformsMoon.model = createModelMatrix(glm::vec4(3.0f, 0.0f, 0.0f, 0.0f) * uniformsMoon.model, glm::vec3(0.0f, rotation * 2, 0.0f), glm::vec3(0.3f));
        uniformsRing.model = createModelMatrix(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, rotation, 0.0f), glm::vec3(0.5f));

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        // Limpiar la ventana
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        //render(vertexArraySphere, uniformsPlanet, Model::Earth);
        //render(vertexArraySphere, uniformsMoon, Model::Moon);
        //render(vertexArraySphere, uniformsPlanet, Model::Venus);
        //render(vertexArraySphere, uniformsPlanet, Model::Saturn);
        //render(vertexArrayCircle, uniformsRing, Model::Ring);
        //render(vertexArraySphere, uniformsPlanet, Model::Sun);

        // Actualizar la ventana
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
