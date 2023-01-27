#include "camera.h"

Camera::Camera(float aspectRatio) {
    float focal       = 1.0f;
    float height      = 1.0f;
    float width       = height * aspectRatio;
    
    m_horizontal      = glm::vec3(width, 0, 0);
    m_vertical        = glm::vec3(0, height, 0);
    m_lowerLeftCorner = m_origin - m_horizontal / 2.0f - m_vertical / 2.0f - glm::vec3(0, 0, focal);
}

Ray Camera::GetRay(float u, float v) {
    Ray ray;
    v = 1 - v;
    ray.origin    = m_origin;
    ray.direction = m_lowerLeftCorner + u * m_horizontal + v * m_vertical - m_origin;
    return ray;
}
