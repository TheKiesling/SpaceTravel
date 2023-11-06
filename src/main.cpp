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
#include <omp.h>
#include "FastNoise.h"
#include "models.h"

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

void clear(SDL_Renderer* renderer, const Camera& camera) {
    FastNoiseLite noise;
    noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  
    SDL_RenderClear(renderer);

    #pragma omp parallel for 
    for (int y = 0; y < WINDOW_HEIGHT; y++) {
        for (int x = 0; x < WINDOW_WIDTH; x++) {
            float noiseValue = noise.GetNoise((x + camera.center.x) * 30.0f, (y + camera.center.y) * 30.0f);

            if (noiseValue > 0.9f) {
                SDL_SetRenderDrawColor(renderer, 225, 225, 225, 225);
                SDL_RenderDrawPoint(renderer, x, y);
            }
        }
    }
}

void movingToAPlanet(SDL_Renderer* renderer, const Camera& camera, Uniforms& spacecraftUniforms) {
    FastNoiseLite noise;
    noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);

    Uint32 startTime = SDL_GetTicks(); 
    const Uint32 animationDuration = 750; 

    while (true) {
        Uint32 currentTime = SDL_GetTicks();  

        Uint32 elapsedTime = currentTime - startTime;

        if (elapsedTime >= animationDuration) {
            break; 
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        for (int y = 0; y < WINDOW_HEIGHT; y += 2) {
            for (int x = 0; x < WINDOW_WIDTH; x += 2) {
                float noiseValue = noise.GetNoise((x + camera.center.x) * 30.0f, (y + camera.center.y) * 30.0f);

                if (noiseValue > 0.75f) {
                    SDL_SetRenderDrawColor(renderer, 225, 225, 225, 225);

                    if (x < WINDOW_WIDTH / 2 && y < WINDOW_HEIGHT / 2)
                    SDL_RenderDrawLine(renderer, x - elapsedTime / 100, y - elapsedTime / 100, x, y );

                    else if (x > WINDOW_WIDTH / 2 && y < WINDOW_HEIGHT / 2)
                    SDL_RenderDrawLine(renderer, x + elapsedTime / 100, y - elapsedTime / 100, x, y);

                    else if (x < WINDOW_WIDTH / 2 && y > WINDOW_HEIGHT / 2)
                    SDL_RenderDrawLine(renderer, x - elapsedTime / 100, y + elapsedTime / 100, x, y);

                    else if (x > WINDOW_WIDTH / 2 && y > WINDOW_HEIGHT / 2)
                    SDL_RenderDrawLine(renderer, x + elapsedTime / 100, y + elapsedTime / 100, x, y);
                }
            }
        }

        SDL_RenderPresent(renderer);

        SDL_Delay(10);  
    }
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

    viewport = glm::scale(viewport, glm::vec3(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f, 0.5f));

    viewport = glm::translate(viewport, glm::vec3(1.0f, 1.0f, 0.5f));

    return viewport;
}

std::vector<std::vector<Vertex>> primitiveAssembly(const std::vector<Vertex>& transformedVertexArray) {
    std::vector<std::vector<Vertex>> assembledVertexArray;
    #pragma omp parallel for 
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
    #pragma omp parallel for 
    for (const std::vector<Vertex>& triangleVertex : assembledVertexArray) {
        std::vector<Fragment> triangleFragments = triangle(triangleVertex[0], triangleVertex[1], triangleVertex[2]);
        fragments.insert(fragments.end(), triangleFragments.begin(), triangleFragments.end());
    }
    return fragments;
}

void render(const std::vector<Vertex>& vertexArray, const Uniforms& uniforms, const Model& planet) {
    // Vertex Shader
    std::vector<Vertex> transformedVertexArray;
    #pragma omp parallel for 
    for (const Vertex& vertex : vertexArray) {
        transformedVertexArray.push_back(vertexShader(vertex, uniforms));
    }

    // Primitive Assembly
    std::vector<std::vector<Vertex>> assembledVertexArray = primitiveAssembly(transformedVertexArray);

    // Rasterization
    std::vector<Fragment> fragments = rasterize(assembledVertexArray);
    
    // Fragment Shader
    #pragma omp parallel for 
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

    std::vector<glm::vec3> modelVertices, modelVerticesCircle, modelVerticesSpacecraft;
    std::vector<glm::vec2> modelTextureCoords, modelTextureCoordsCircle, modelTextureCoordsSpacecraft;
    std::vector<glm::vec3> modelNormals, modelNormalsCircle, modelNormalsSpacecraft;
    std::vector<Face> modelFaces, modelFacesCircle, modelFacesSpacecraft;
    bool successSphere = loadOBJ("../models/sphere.obj", modelVertices, modelTextureCoords, modelNormals, modelFaces);
    bool succesCircle = loadOBJ("../models/circle.obj", modelVerticesCircle, modelTextureCoordsCircle, modelNormalsCircle, modelFacesCircle);
    bool succesSpacecraft = loadOBJ("../models/lab3.obj", modelVerticesSpacecraft, modelTextureCoordsSpacecraft, modelNormalsSpacecraft, modelFacesSpacecraft);

    std::vector<Vertex> vertexArraySphere = setupVertexArray(modelVertices, modelTextureCoords, modelNormals, modelFaces);
    std::vector<Vertex> vertexArrayCircle = setupVertexArray(modelVerticesCircle, modelTextureCoordsCircle, modelNormalsCircle, modelFacesCircle);
    std::vector<Vertex> vertexArraySpacecraft = setupVertexArray(modelVerticesSpacecraft, modelTextureCoordsSpacecraft, modelNormalsSpacecraft, modelFacesSpacecraft);

    Camera camera {
        glm::vec3(0.0f, 0.0f, 3.0f),
        glm::vec3(0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    };

    Uniforms uniformsEarth{
        createModelMatrix(glm::vec3(3.0f, 0.0f, -20.0f), glm::vec3(0.0f), glm::vec3(1.0f)),
        createViewMatrix(camera),
        createProjectionMatrix(),
        createViewportMatrix()
    };

    Uniforms uniformsMoon{
        createModelMatrix(glm::vec3(3.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.4f)),
        createViewMatrix(camera),
        createProjectionMatrix(),
        createViewportMatrix()
    };

    Uniforms uniformsSpacecraft{
        createModelMatrix(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.2f)),
        createViewMatrix(camera),
        createProjectionMatrix(),
        createViewportMatrix()
    };

    Uniforms uniformsSun{
        createModelMatrix(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(5.0f)),
        createViewMatrix(camera),
        createProjectionMatrix(),
        createViewportMatrix()
    };

    Uniforms uniformsVenus{
        createModelMatrix(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.8f)),
        createViewMatrix(camera),
        createProjectionMatrix(),
        createViewportMatrix()
    };

    Uniforms uniformsSaturn{
        createModelMatrix(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.4f)),
        createViewMatrix(camera),
        createProjectionMatrix(),
        createViewportMatrix()
    };
    
    Uniforms uniformsRing{
        createModelMatrix(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.8f)),
        createViewMatrix(camera),
        createProjectionMatrix(),
        createViewportMatrix()
    };
    

    bool running = true;
    SDL_Event event;

    int Translation = 0.0f;
    int Rotation = 0.0f;

    glm::vec3 spacecraftPosition = glm::vec3(0.0f, 0.0f, -50.0f);
    float spacecraftSpeed = 2.0f; 
    glm::vec3 viewSpacecraft = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 cameraOffset = glm::vec3(0.0f, 0.0f, -15.0f); 

    glm::vec3 earthPosition = glm::vec3(0.0f, 0.0f, -20.0f);
    glm::vec3 moonPosition = glm::vec3(0.0f, 0.0f, -20.0f);
    glm::vec3 sunPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 venusPosition = glm::vec3(0.0f, 0.0f, -10.0f);
    glm::vec3 saturnPosition = glm::vec3(0.0f, 0.0f, -30.0f);
    glm::vec3 ringPosition = glm::vec3(0.0f, 0.0f, -30.0f);

    float moonOrbitSpeed = 1.0f; 
    float moonDistance = 1.0f; 
    float moonOrbitAngle = 0.0f;

    Model model = Model::Spacecraft;

    while (running) {
        clearZBuffer();
        
        Translation += 1.0f;
        Rotation += 1.0f;

        const Uint8 *keys = SDL_GetKeyboardState(NULL);

        if (keys[SDL_SCANCODE_LEFT]) {
            model = Model::Spacecraft;
                spacecraftPosition.x += spacecraftSpeed;
        }
        if (keys[SDL_SCANCODE_RIGHT]) {
            model = Model::Spacecraft;
                spacecraftPosition.x -= spacecraftSpeed;
        }
        if (keys[SDL_SCANCODE_UP]) {
            model = Model::Spacecraft;
            if (sunPosition.z > spacecraftPosition.z + 10.0f) {
                spacecraftPosition.z += spacecraftSpeed;
            }
        }
        if (keys[SDL_SCANCODE_DOWN]) {
            model = Model::Spacecraft;
            if (sunPosition.z > spacecraftPosition.z - 10.0f) {
                spacecraftPosition.z -= spacecraftSpeed;
            }
        }
        if (keys[SDL_SCANCODE_1]) {
            model = Model::Earth;
            movingToAPlanet(renderer, camera, uniformsSpacecraft);
        }
        if (keys[SDL_SCANCODE_2]) {
            model = Model::Venus;
            movingToAPlanet(renderer, camera, uniformsSpacecraft);
        }
        if (keys[SDL_SCANCODE_3]) {
            model = Model::Saturn;
            movingToAPlanet(renderer, camera, uniformsSpacecraft);
        }
        if (keys[SDL_SCANCODE_4]) {
            model = Model::Spacecraft;
            spacecraftPosition.x = 0.0f;
            spacecraftPosition.y = 0.0f;
            spacecraftPosition.z = -50.0f;
            movingToAPlanet(renderer, camera, uniformsSpacecraft);
        }

        switch (model){
            case Model::Earth:
                spacecraftPosition.x = earthPosition.x;
                spacecraftPosition.y = earthPosition.y;
                spacecraftPosition.z = earthPosition.z - 5.0f;
                break;
            case Model::Venus:
                spacecraftPosition.x = venusPosition.x;
                spacecraftPosition.y = venusPosition.y;
                spacecraftPosition.z = venusPosition.z - 5.0f;
                break;
            case Model::Saturn:
                spacecraftPosition.x = saturnPosition.x;
                spacecraftPosition.y = saturnPosition.y;
                spacecraftPosition.z = saturnPosition.z - 5.0f;
                break;
            default:
                break;
        }

        moonOrbitAngle += moonOrbitSpeed;

        uniformsSpacecraft.model = createModelMatrix(spacecraftPosition, glm::vec3(0.0f), glm::vec3(0.1f));

        uniformsSun.model = createModelMatrix(sunPosition, glm::vec3(0.0f, Rotation, 0.0f), glm::vec3(10.0f));

        earthPosition.x = sunPosition.x + 200.0f * glm::cos(glm::radians(moonOrbitAngle));
        earthPosition.z = sunPosition.z + 200.0f * glm::sin(glm::radians(moonOrbitAngle));

        uniformsEarth.model = createModelMatrix(earthPosition, glm::vec3(0.0f, Rotation, 0.0f), glm::vec3(1.0f));

        moonPosition.x = earthPosition.x + moonDistance * glm::cos(glm::radians(moonOrbitAngle * 4));
        moonPosition.z = earthPosition.z + moonDistance * glm::sin(glm::radians(moonOrbitAngle * 4));

        uniformsMoon.model = createModelMatrix(moonPosition, glm::vec3(0.0f, Rotation * 2, 0.0f), glm::vec3(0.4f));

        venusPosition.x = sunPosition.x + 100.0f * glm::cos(glm::radians(moonOrbitAngle * 2));
        venusPosition.z = sunPosition.z + 100.0f * glm::sin(glm::radians(moonOrbitAngle * 2));

        uniformsVenus.model = createModelMatrix(venusPosition, glm::vec3(0.0f, Rotation, 0.0f), glm::vec3(0.8f));

        saturnPosition.x = sunPosition.x + 400.0f * glm::cos(glm::radians(moonOrbitAngle * 0.5));
        saturnPosition.z = sunPosition.z + 400.0f * glm::sin(glm::radians(moonOrbitAngle * 0.5));

        uniformsSaturn.model = createModelMatrix(saturnPosition, glm::vec3(0.0f, Rotation, 0.0f), glm::vec3(1.4f));

        ringPosition.x = saturnPosition.x;
        ringPosition.z = saturnPosition.z;

        uniformsRing.model = createModelMatrix(ringPosition, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.8f));

        camera.position = spacecraftPosition + cameraOffset;
        camera.center = spacecraftPosition;

        uniformsSpacecraft.view = createViewMatrix(camera);
        uniformsEarth.view = createViewMatrix(camera);
        uniformsMoon.view = createViewMatrix(camera);
        uniformsSun.view = createViewMatrix(camera);
        uniformsVenus.view = createViewMatrix(camera);
        uniformsSaturn.view = createViewMatrix(camera);
        uniformsRing.view = createViewMatrix(camera);

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        clear(renderer, camera);

        render(vertexArraySpacecraft, uniformsSpacecraft, Model::Spacecraft);
        render(vertexArraySphere, uniformsEarth, Model::Earth);
        render(vertexArraySphere, uniformsMoon, Model::Moon);
        render(vertexArraySphere, uniformsSun, Model::Sun);
        render(vertexArraySphere, uniformsVenus, Model::Venus);
        render(vertexArraySphere, uniformsSaturn, Model::Saturn);
        render(vertexArrayCircle, uniformsRing, Model::Ring);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
