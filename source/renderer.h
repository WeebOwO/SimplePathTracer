#pragma once

#include <memory>

#include <SDL.h>
#include <glm/fwd.hpp>
#include <stdint.h>

#include "camera.h"
#include "geometry.h"
#include "ray.h"
#include "scene.h"

class Renderer {
public:
    Renderer(int viewPortWidth, int viewPortHeight);
    ~Renderer();
    void Render();
    void OnResize(uint32_t width, uint32_t height);
    void SetActiveScene(const Scene& scene) { m_activeScene = std::make_shared<Scene>(scene); }

private:
    int        Init();
    void       RenderImage();
    void       DrawPixel(int x, int y, const glm::vec4& color);
    void       PixelShader(uint32_t x, uint32_t y);
    glm::vec3  RayColor(Ray& ray);
    HitPayload TraceRay(const Ray& ray);

private:
    uint32_t               m_viewPortWidth{900};
    uint32_t               m_viewPortHeight{600};
    SDL_Window*            m_window;
    SDL_Renderer*          m_renderer;
    SDL_Texture*           m_swapBuffer;
    uint32_t*              m_frameBuffer;
    Camera                 m_camera;
    std::shared_ptr<Scene> m_activeScene;
    float                  m_renderTime;
};