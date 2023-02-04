#include "camera.h"
#include "misc.h"
#include <cmath>

Camera::Camera(const glm::vec3& origin, const glm::vec3& lookat, const glm::vec3& updir, float aspectRatio, float verticalFov) {
    float theta = misc::DegreeToRadians(verticalFov);
    float h = tan(theta / 2.0f);
    
    float height = 2 * h;
    float width  = height * aspectRatio;

    glm::vec3 w = glm::normalize(origin - lookat);
    glm::vec3 u = glm::normalize(glm::cross(updir, w));
    glm::vec3 v = glm::cross(w, u);

    m_origin = origin;    
    m_horizontal      = width * u;
    m_vertical        = height * v;
    m_lowerLeftCorner = m_origin - m_horizontal / 2.0f - m_vertical / 2.0f - w;
}

Ray Camera::GetRay(float u, float v) {
    Ray ray;
    ray.origin    = m_origin;
    ray.direction = m_lowerLeftCorner + u * m_horizontal + v * m_vertical - m_origin;
    return ray;
}
