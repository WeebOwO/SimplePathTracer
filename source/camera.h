#pragma once

#include "glm/fwd.hpp"
#include "ray.h"

class Camera {
public:
    Camera() = default;
    Camera(const glm::vec3& origin, const glm::vec3& lookat, const glm::vec3& updir,
           float aspectRatio, float verticleFov);
    Ray  GetRay(float u, float v);

private:
    glm::vec3 m_origin{0, 0, 0};
    glm::vec3 m_updir;
    glm::vec3 m_lookat;
    glm::vec3 m_lowerLeftCorner;
    glm::vec3 m_vertical;
    glm::vec3 m_horizontal;
    bool      m_defocus{false};
};