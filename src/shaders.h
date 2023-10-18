#pragma once
#include <vector>
#include "uniforms.h"
#include "fragment.h"
#include "color.h"
#include "vertex.h"
#include "FastNoise.h"

Vertex vertexShader(const Vertex& vertex, const Uniforms& uniforms) {
    glm::mat4 MVP = uniforms.projection * uniforms.view * uniforms.model;
    glm::vec4 relativeVertex = MVP * glm::vec4(vertex.position, 1.0f);
    glm::vec4 transformedVertex = uniforms.viewport * uniforms.projection * uniforms.view * uniforms.model * glm::vec4(vertex.position, 1.0f);
    glm::vec3 screenVertex = transformedVertex/transformedVertex.w;
    glm::vec3 normal = glm::normalize(glm::mat3(uniforms.model) * vertex.normal);
    return Vertex{screenVertex, vertex.color, normal, vertex.textureCoord, vertex.position, relativeVertex.w > 0.0f};
}

Fragment fragmentShader(Fragment& fragment) {
    Color color = fragment.color * fragment.intensity;
    fragment.color = color;
    return fragment;
}

Fragment sun(Fragment& fragment){
    Color color;

    glm::vec3 red = glm::vec3(255.0f, 0.0f, 0.0f);
    glm::vec3 rose = glm::vec3(255.0f, 90.0f, 0.0f);
    glm::vec3 pink = glm::vec3(255.0f, 100.0f, 0.0f);

    float noise = glm::fract(sin(glm::dot(fragment.position, glm::vec2(12.9898, 78.233))) * 43758.5453) * 0.5f + 0.5f;

    float distanceFromCenter = glm::length(fragment.original);
    red *= 0.4f - distanceFromCenter;
    rose *= 0.6f - distanceFromCenter;
    pink *= 0.25f - distanceFromCenter;

    glm::vec3 tmpColor = (red + rose + pink) * glm::vec3(0.4f, 0.1f, 0.0f);

    color = Color(tmpColor.r * noise, tmpColor.g * noise, tmpColor.b * noise);

    float glow = 1.0f + 1.0f * sin(1.0f);
    fragment.color = color * fragment.intensity * glow;
    return fragment;
}

Fragment saturn(Fragment& fragment){
    Color color;

    glm::vec3 wheat = glm::vec3(237.0f, 219.0f, 173.0f);

    float stripePattern = glm::abs(glm::cos(fragment.original.y * 40.0f)) * 17.0f;
    float secondStripePattern = glm::abs(glm::cos(fragment.original.y * 25.0f)) * 25.0f;

    glm::vec3 final = wheat + stripePattern - secondStripePattern;

    color = Color(final.r, final.g, final.b);

    fragment.color = color * fragment.intensity;
    return fragment;
}

Fragment ring(Fragment& fragment){
    Color color;

    glm::vec3 almond = glm::vec3(237.0f, 219.0f, 200.0f);

    float stripePattern = glm::abs(glm::cos(fragment.original.z * 40.0f)) * 17.0f;
    float secondStripePattern = glm::abs(glm::cos(fragment.original.x * 25.0f)) * 25.0f;

    glm::vec3 final = almond + stripePattern - secondStripePattern;

    color = Color(final.r, final.g, final.b);

    fragment.color = color * fragment.intensity;
    return fragment;
}

Fragment earth(Fragment& fragment){
    Color color;

    FastNoiseLite noiseGenerator;
    noiseGenerator.SetNoiseType(FastNoiseLite::NoiseType_Perlin);

    glm::vec3 uvw = glm::vec3(fragment.original.x, fragment.original.y, fragment.original.z);

    float ox = 300.0f; 
    float oy = 200.0f;
    float oz = 250.0f;
    float zoom = 350.0f;

    float noiseValue = noiseGenerator.GetNoise((uvw.x + ox) * zoom, (uvw.y + oy) * zoom, (uvw.z + oz) * zoom);

    glm::vec3 oceanColor = glm::vec3(85.0f / 255.0f, 131.0f / 255.0f, 194.0f / 255.0f);
    glm::vec3 beachColor = glm::vec3(229.0f / 255.0f, 196.0f / 255.0f, 148.0f / 255.0f);
    glm::vec3 grassColor = glm::vec3(107.0f / 255.0f, 160.0f / 255.0f, 84.0f / 255.0f);
    glm::vec3 forestColor = glm::vec3(61.0f / 255.0f, 113.0f / 255.0f, 80.0f / 255.0f);
    glm::vec3 mountainColor = glm::vec3(119.0f / 255.0f, 107.0f / 255.0f, 87.0f / 255.0f);
    glm::vec3 snowColor = glm::vec3(1.0f, 1.0f, 1.0f);

    glm::vec3 tmpColor;
    if (noiseValue < 0.10f) {
        tmpColor = oceanColor;
    } else if (noiseValue < 0.15f) {
        tmpColor = beachColor;
    } else if (noiseValue < 0.3f) {
        tmpColor = grassColor;
    } else if (noiseValue < 0.45f) {
        tmpColor = forestColor;
    } else if (noiseValue < 0.7f) {
        tmpColor = mountainColor;
    } else {
        tmpColor = snowColor;
    }

    if (fragment.original.y < -0.4f || fragment.original.y > 0.4f) {
        tmpColor = snowColor;
    }

    color = Color(tmpColor.x * 255.0f, tmpColor.y * 255.0f, tmpColor.z * 255.0f);
    fragment.color = color * fragment.intensity;

    return fragment;
}

Fragment moon(Fragment& fragment){
    Color color;

    FastNoiseLite noiseGenerator;
    noiseGenerator.SetNoiseType(FastNoiseLite::NoiseType_Value);

    glm::vec3 uvw = glm::vec3(fragment.original.x, fragment.original.y, fragment.original.z);

    float ox = 300.0f; 
    float oy = 200.0f;
    float oz = 250.0f;
    float zoom = 300.0f;

    float noiseValue = noiseGenerator.GetNoise((uvw.x + ox) * zoom, (uvw.y + oy) * zoom, (uvw.z + oz) * zoom);

    glm::vec3 silver = glm::vec3(201.0f / 255.0f, 201.0f / 255.0f, 201.0f / 255.0f);
    glm::vec3 gray = glm::vec3(148.0f / 255.0f, 144.0f / 255.0f, 141.0f / 255.0f);
    glm::vec3 white = glm::vec3(229.0f / 255.0f, 229.0f / 255.0f, 229.0f / 255.0f);
    glm::vec3 platinum = glm::vec3(220.0f / 255.0f, 220.0f / 255.0f, 220.0f / 255.0f);
    glm::vec3 dark = glm::vec3(49.0f / 255.0f, 48.0f / 255.0f, 46.0f / 255.0f);

    glm::vec3 tmpColor;
    if (noiseValue < 0.0f) {
        tmpColor = silver;
    } else if (noiseValue < 0.25f) {
        tmpColor = gray;
    } else if (noiseValue < 0.5f) {
        tmpColor = dark;
    } else if (noiseValue < 0.75f) {
        tmpColor = platinum;
    } else  {
        tmpColor = white;
    }

    color = Color(tmpColor.x * 255.0f, tmpColor.y * 255.0f, tmpColor.z * 255.0f);
    fragment.color = color * fragment.intensity;

    return fragment;
}

Fragment venus(Fragment& fragment){
    Color color;

    FastNoiseLite noiseGenerator;
    noiseGenerator.SetNoiseType(FastNoiseLite::NoiseType_Value);

    glm::vec3 uvw = glm::vec3(fragment.original.x, fragment.original.y, fragment.original.z);

    float ox = 300.0f; 
    float oy = 200.0f;
    float oz = 200.0f;
    float zoom = 400.0f;

    float noiseValue = noiseGenerator.GetNoise((uvw.x + ox) * zoom, (uvw.y + oy) * zoom, (uvw.z + oz) * zoom);

    glm::vec3 wheat = glm::vec3(254.0f / 255.0f, 214.0f / 255.0f, 119.0f / 255.0f);
    glm::vec3 jasper = glm::vec3(205.0f / 255.0f, 86.0f / 255.0f, 54.0f / 255.0f);
    glm::vec3 persian = glm::vec3(234.0f / 255.0f, 145.0f / 255.0f, 79.0f / 255.0f);

    glm::vec3 tmpColor;
    if (noiseValue < -0.1f) {
        tmpColor = wheat;
    } else if (noiseValue < 0.4f) {
        tmpColor = persian;
    } else {
        tmpColor = jasper;
    } 

    color = Color(tmpColor.x * 255.0f, tmpColor.y * 255.0f, tmpColor.z * 255.0f);
    fragment.color = color * fragment.intensity;

    return fragment;
}
