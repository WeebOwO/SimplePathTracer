#pragma once

#include "glm/fwd.hpp"

#include "ray.h"

class Camera {
public:
    Camera() = default;
    Camera(float aspectRatio);
    Ray GetRay(float u, float v);

private:
    float     m_focalLength{1.0f};
    glm::vec3 m_origin{0, 0, 0};
    glm::vec3 m_lowerLeftCorner;
    glm::vec3 m_vertical;
    glm::vec3 m_horizontal;
};