#pragma once

#include <SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>

#include "ray.h"
#include "scene.h"

class Renderer {
public:
    Renderer(int viewPortWidth, int viewPortHeight);
    ~Renderer();
    void Render();
    void OnResize(uint32_t width, uint32_t height);
    void SetActiveScene(const Scene& scene) {m_activeScene = std::make_shared<Scene>(scene);}

private:
    int        Init();
    HitPayload TraceRay(const Ray& ray);
    HitPayload ClosestHit(const Ray& ray, float hitDistance, int objectIndex);
    HitPayload Miss(const Ray& ray);
    void       RenderClear();
    void       RenderImage();
    void       DrawPixel(int x, int y, const glm::vec4& color);
    void       PixelShader(uint32_t x, uint32_t y);

private:
    uint32_t               m_viewPortWidth{900};
    uint32_t               m_viewPortHeight{600};
    SDL_Window*            m_window;
    SDL_Renderer*          m_renderer;
    SDL_Texture*           m_swapBuffer;
    uint32_t*              m_frameBuffer;
    std::shared_ptr<Scene> m_activeScene;
    glm::vec3              m_backColor{0.0f, 0.0f, 0.0f};
    float                  m_renderTime;
};