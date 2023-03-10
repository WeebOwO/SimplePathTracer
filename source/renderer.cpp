#include "renderer.h"

#include <algorithm>
#include <execution>
#include <iostream>
#include <limits>
#include <numeric>
#include <ranges>
#include <vector>
#include <cmath>

#include "material.h"
#include "misc.h"
#include "pdf.h"

Renderer::Renderer(int viewPortWidth, int viewPortHeight)
    : m_viewPortWidth(viewPortWidth), m_viewPortHeight(viewPortHeight), m_camera(nullptr) {
    Init();
}

Renderer::~Renderer() {
    // Clean up SDL context
    SDL_DestroyWindow(m_window);
    SDL_DestroyRenderer(m_renderer);
    SDL_Quit();
    delete[] m_frameBuffer;
}

int Renderer::Init() {
    // SDL init part
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0) {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }
    SDL_WindowFlags window_flags =
        (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    m_window = SDL_CreateWindow("Raytracer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                m_viewPortWidth, m_viewPortHeight, window_flags);
    m_renderer =
        SDL_CreateRenderer(m_window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    m_swapBuffer =
        SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
                          m_viewPortWidth, m_viewPortHeight);
    if (m_renderer == nullptr) {
        SDL_Log("Error creating SDL_Renderer!");
        return 0;
    }
    m_frameBuffer = new uint32_t[m_viewPortWidth * m_viewPortHeight];
    m_colorBuffer.resize(m_viewPortWidth * m_viewPortHeight);
    return 1;
}

void Renderer::Render() {
    if (m_camera == nullptr) {
        SDL_Log("Please bind cemera!\n");
        return;
    }
    if (m_activeScene == nullptr) {
        SDL_Log("Please bind scene!\n");
        return;
    }
    bool done = false;
    // main loop
    while (!done) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) done = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE &&
                event.window.windowID == SDL_GetWindowID(m_window)) {
                done = true;
            }
        }
        RenderImage();
    }
}

void Renderer::RenderImage() {
    auto horizontalRange = std::ranges::views::iota((uint32_t)0, m_viewPortWidth);
    auto verticalRange   = std::ranges::views::iota((uint32_t)0, m_viewPortHeight);
    int  spp             = 1000000;
    for (int i = 0; i < spp; ++i) {
        auto startTick = SDL_GetTicks();
        for (auto y : verticalRange) {
            std::for_each(std::execution::par, horizontalRange.begin(), horizontalRange.end(),
                          [&](int x) { PixelShader(x, y, 1.0f / i); });
        }
        auto endTick = SDL_GetTicks();
        SDL_Log("Per spp tick is %d ms", endTick - startTick);
        SDL_UpdateTexture(m_swapBuffer, nullptr, m_frameBuffer, sizeof(uint32_t) * m_viewPortWidth);
        SDL_RenderCopy(m_renderer, m_swapBuffer, nullptr, nullptr);
        SDL_RenderPresent(m_renderer);
    }
}

void Renderer::OnResize(uint32_t width, uint32_t height) {
    if (width == m_viewPortWidth && height == m_viewPortHeight) { return; }
    m_viewPortWidth = width, m_viewPortHeight = height;
    m_frameBuffer = new uint32_t[width * height];
}

void Renderer::PixelShader(uint32_t x, uint32_t y, float scale) {
    // flip y
    auto& color = m_colorBuffer[y * m_viewPortWidth + x];

    // antialiasing
    float nx = x + Random::Float();
    float ny = y + Random::Float();

    float u   = nx * 1.0f / m_viewPortWidth;
    float v   = ny * 1.0f / m_viewPortHeight;
    Ray   ray = m_camera->GetRay(u, 1 - v);

    color += RayColor(ray);

    glm::vec3 result = color * scale;
    DrawPixel(x, y, glm::vec4(glm::sqrt(result), 1.0f));
}

glm::vec3 Renderer::RayColor(Ray& ray) {
    if (Random::Float() > 0.8f) return black;
    HitPayload payload = m_activeScene->Hit(ray);

    if (payload.objectIndex == -1) {
        return black;
    }

    const auto& material = m_activeScene->GetMaterial(payload.objectIndex);
    float pdfValue = 0.25 * pbr::pi, index;

    if (material.type == MaterialType::Light) { return {15.0f, 15.0f, 15.0f}; }

    if (material.type == MaterialType::Diffuse) {        
        CosinePdf p(payload.wordNormal);
        ray.direction = p.Generate();
        float cosine = glm::dot(payload.wordNormal, ray.direction);
        index = cosine < 0 ? 0 : cosine / pbr::pi;
        pdfValue = p.Value(ray.direction);
    }

    if (material.type == MaterialType::Metal) {
        glm::vec3 reflect = glm::reflect(glm::normalize(ray.direction), payload.wordNormal);
        ray.direction     = reflect + material.fuzz * pbr::RandomUnitSphereDir();
    }

    if (material.type == MaterialType::Dielectric) {
        float refractionRatio = 1.5f;
        if (payload.frontFace) { refractionRatio = 1.0f / refractionRatio; }
        glm::vec3 unit = glm::normalize(ray.direction);

        float cosTheta = fmin(glm::dot(-unit, payload.wordNormal), 1.0f);
        float sinTheta = sqrt(1.0f - cosTheta * cosTheta);
        // total internal reflection
        if (refractionRatio * sinTheta > 1.0f ||
            pbr::Reflectance(cosTheta, refractionRatio) > Random::Float()) {
            ray.direction = glm::reflect(unit, payload.wordNormal);
        } else {
            ray.direction = glm::refract(unit, payload.wordNormal, refractionRatio);
        }
    }

#ifdef ONLY_LIGHT
    glm::vec3 onLight = glm::vec3(misc::RandomFloat(213, 343), 554, misc::RandomFloat(227, 332));
    glm::vec3 toLight = onLight - payload.worldPos;
    float distanceSq = glm::dot(toLight, toLight);
    toLight = glm::normalize(toLight);

    if(glm::dot(toLight, payload.wordNormal) < 0) return black;
    constexpr float area = (343-213)*(332-227);
    float lightCos = fabs(toLight.y);
    if(lightCos < 1e-4) return black;
    float lightPdf = distanceSq / (lightCos * area); 
    float pdf = 0.5 * lightPdf + 0.5 * pdfValue;
    ray.direction = toLight;
#endif

    ray.origin = payload.worldPos;
    return material.albedo * RayColor(ray) * index / 0.8f / pdfValue;
}

void Renderer::DrawPixel(int x, int y, const glm::vec4& color) {
    glm::vec4 finalColorVec                    = glm::clamp(color, 0.0f, 1.0f);
    auto      r                                = static_cast<uint8_t>(finalColorVec.r * 255.0f);
    auto      g                                = static_cast<uint8_t>(finalColorVec.g * 255.0f);
    auto      b                                = static_cast<uint8_t>(finalColorVec.b * 255.0f);
    auto      a                                = static_cast<uint8_t>(finalColorVec.a * 255.0f);
    uint32_t  finalColor                       = (a << 24) + (r << 16) + (g << 8) + b;
    *(m_frameBuffer + y * m_viewPortWidth + x) = finalColor;
}