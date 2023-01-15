#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <SDL.h>
#include "ray.h"
#include "scene.h"

class SDL_Window;
class SDL_Renderer;
class SDL_Texture;

class Renderer {
public:
    Renderer(int viewPortWidth, int viewPortHeight);
    ~Renderer();
    void Render();
    void OnResize(uint32_t width, uint32_t height);

private:
    struct HitPayLoad {
        float     hitDistance;
        glm::vec3 worldPos;
        glm::vec3 wordNormal;
        int       objectIndex;
    };

    int        Init();
    HitPayLoad TraceRay(const Ray& ray);
    HitPayLoad ClosestHit(const Ray& ray, float hitDistance, int objectIndex);
    HitPayLoad Miss(const Ray& ray);
    void       RenderPresent();
    void       RenderClear();
    void       RenderImage();
    void       RenderUI();
    void       DrawPixel(int x, int y, const glm::vec4& color);
    void       PixelShader(uint32_t x, uint32_t y);

private:
    uint32_t      m_viewPortWidth{900};
    uint32_t      m_viewPortHeight{600};
    SDL_Window*   m_window;
    SDL_Renderer* m_renderer;
    SDL_Texture*  m_swapBuffer;
    uint32_t*     m_frameBuffer;
    glm::vec3     m_backColor{0.0f, 0.0f, 0.0f};
    float         m_renderTime;
};