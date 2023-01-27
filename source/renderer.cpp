#include "renderer.h"

#include <iostream>
#include <limits>
#include <numeric>
#include <ranges>
#include <execution>
#include <stdint.h>
#include <algorithm>

#include "camera.h"
#include "ray.h"

Renderer::Renderer(int viewPortWidth, int viewPortHeight)
    : m_viewPortWidth(viewPortWidth), m_viewPortHeight(viewPortHeight),
      m_camera(viewPortWidth * 1.0f / viewPortHeight) {
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

    // camera init
    return 1;
}

void Renderer::Render() {
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

void Renderer::RenderClear() {
    auto r = static_cast<int>(m_backColor.r * 255.0f);
    auto g = static_cast<int>(m_backColor.g * 255.0f);
    auto b = static_cast<int>(m_backColor.b * 255.0f);
    auto a = static_cast<int>(1.0f * 255.0f);
    SDL_SetRenderDrawColor(m_renderer, r, g, b, a);
    SDL_RenderClear(m_renderer);
}

void Renderer::RenderImage() {
    auto horizontalRange = std::ranges::views::iota((uint32_t)0, m_viewPortWidth);
    auto verticalRange   = std::ranges::views::iota((uint32_t)0, m_viewPortHeight);

    for (auto y : verticalRange) {
        std::for_each(std::execution::par, horizontalRange.begin(), horizontalRange.end(), [&](int x){
            PixelShader(x, y);
        });
    }
    
    SDL_UpdateTexture(m_swapBuffer, nullptr, m_frameBuffer, sizeof(uint32_t) * m_viewPortWidth);
    SDL_RenderCopy(m_renderer, m_swapBuffer, nullptr, nullptr);
    SDL_RenderPresent(m_renderer);
}

void Renderer::OnResize(uint32_t width, uint32_t height) {
    if (width == m_viewPortWidth && height == m_viewPortHeight) { return; }
    m_viewPortWidth = width, m_viewPortHeight = height;
    m_frameBuffer = new uint32_t[width * height];
}

void Renderer::PixelShader(uint32_t x, uint32_t y) {
    // flip y
    float u = x * 1.0f / m_viewPortWidth;
    float v = y * 1.0f / m_viewPortHeight;

    // hit something or just draw background color
    Ray ray = m_camera.GetRay(u, v);
    HitPayload payload  = TraceRay(ray);
    glm::vec3 lightdir = glm::normalize(glm::vec3(-1.0f, -1.0f, -1.0f));
    if (payload.objectIndex != -1) {
        int  index  = payload.objectIndex;
        glm::vec3 albedo = m_activeScene->materials[index].albedo;
        glm::vec3 diffuse = glm::dot(-lightdir, payload.wordNormal) * albedo;
        // diffuse part
        DrawPixel(x, y, glm::vec4(diffuse, 1.0f));
        return;
    }

    glm::vec4 blue       = glm::vec4(0.5f, 0.7f, 1.0f, 1.0f);
    glm::vec4 white      = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec4 background = (1 - v) * blue + v * white;
    DrawPixel(x, y, background);
}

HitPayload Renderer::TraceRay(const Ray& ray) {
    HitPayload payload;
    float closestHitTime     = std::numeric_limits<float>::max();
    payload.objectIndex = -1;
    payload.hitDistance = std::numeric_limits<float>::max();

    for (size_t i = 0; i < m_activeScene->objects.size(); ++i) {
        auto& hitObejct = m_activeScene->objects[i];
        // this intersection method is only use for sphere
        // need abstract to hitable object
        if (auto hitResult = hitObejct->Hit(ray)) {
            float hitTime = hitResult->hitTime;
            if (hitTime < closestHitTime) {
                payload = hitResult.value();
                payload.objectIndex = i;
                closestHitTime = hitTime;
            }
        }
    }

    return payload;
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