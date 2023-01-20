#include "renderer.h"

#include <iostream>
#include <limits>
#include <numeric>

#include "ray.h"
#include "scene.h"

Renderer::Renderer(int viewPortWidth, int viewPortHeight)
    : m_viewPortWidth(viewPortWidth), m_viewPortHeight(viewPortHeight) {
    Init();
}

Renderer::~Renderer() {
    // Clean up SDL context
    SDL_DestroyWindow(m_window);
    SDL_DestroyRenderer(m_renderer);
    SDL_Quit();
    delete [] m_frameBuffer;
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
    return 1;
}

void Renderer::Render() {
    bool done = false;
    //main loop
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
    for (uint32_t y = 0; y < m_viewPortHeight; ++y) {
        for (uint32_t x = 0; x < m_viewPortWidth; ++x) {
            PixelShader(x, y);
        }
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
    Ray ray;
    float aspect = 1.0f * m_viewPortWidth / m_viewPortHeight;
    glm::vec2 coord = glm::vec2(x * 1.0f / m_viewPortWidth, y * 1.0f / m_viewPortHeight);
    float t = coord.y;

    coord = coord * 2.0f - 1.0f;
    coord.x *= aspect;

    ray.origin = glm::vec3(0.0f, 0.0f, 2.0f);
    ray.direction = glm::vec3(coord.x, coord.y, -1.0f);
 
    HitPayload payload = TraceRay(ray);
    if(payload.objectIndex != -1) {
        int index = payload.objectIndex;
        glm::vec3 color = m_activeScene->materials[index].albedo;
        DrawPixel(x, y, glm::vec4(color, 1.0f));
        return;
    }

    glm::vec4 blue = glm::vec4(0.5f, 0.7f, 1.0f, 1.0f);
    glm::vec4 white = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec4 background = (1 - t) * blue + t * white;
    DrawPixel(x, y, glm::vec4(background));

}

HitPayload Renderer::TraceRay(const Ray& ray) {
    float hitDistance = std::numeric_limits<float>::max();
    int closestObject = -1;
    
    for(size_t i = 0; i < m_activeScene->spheres.size(); ++i) {
        const Sphere& sphere = m_activeScene->spheres[i];
        glm::vec3 origin = ray.origin - sphere.position;

        float a = glm::dot(ray.direction, ray.direction);
        float b = 2.0f * glm::dot(origin, ray.direction);
        float c = glm::dot(origin, origin) - sphere.radius * sphere.radius;
        
        float discriminant = b * b - 4.0f * a * c;
        
        float closestT = (-b - glm::sqrt(discriminant)) / (2.0f * a);
        if(closestT > 0.0f && closestT < hitDistance) {
            hitDistance = closestT;
            closestObject = i;
        }
    }
    return closestObject == -1 ? Miss(ray) : ClosestHit(ray, hitDistance, closestObject);
}

HitPayload Renderer::Miss(const Ray& ray) {
    HitPayload payload;
    payload.objectIndex = -1;
    payload.hitDistance = -1.0f;
    return payload;
}

HitPayload Renderer::ClosestHit(const Ray& ray, float hitDistance, int objectIndex) {
    HitPayload payload;
    payload.hitDistance = hitDistance;
    payload.objectIndex = 0;

    const Sphere& closestSphere = m_activeScene->spheres[objectIndex];

    glm::vec3 origin = ray.origin - closestSphere.position;
    payload.worldPos = origin + ray.direction * hitDistance;
    payload.wordNormal = glm::normalize(payload.worldPos);
    return payload;
}


void Renderer::DrawPixel(int x, int y, const glm::vec4& color) {
    auto finalColorVec = glm::clamp(color, 0.0f, 1.0f);
    auto r = static_cast<uint8_t>(finalColorVec.r * 255.0f);
    auto g = static_cast<uint8_t>(finalColorVec.g * 255.0f);
    auto b = static_cast<uint8_t>(finalColorVec.b * 255.0f);
    auto a = static_cast<uint8_t>(finalColorVec.a * 255.0f);
    uint32_t finalColor = (a << 24) + (r << 16) + (g << 8) + b;
    *(m_frameBuffer + y * m_viewPortWidth + x) = finalColor;
}