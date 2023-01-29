#include "geometry.h"

Sphere::Sphere(glm::vec3 positon, float radius, int index)
    : m_position(positon), m_radius(radius), m_materialIndex(index) {}

std::optional<float> Sphere::Hit(const Ray& ray) {
    glm::vec3 origin       = ray.origin - m_position;
    float     a            = glm::dot(ray.direction, ray.direction);
    float     b            = 2.0f * glm::dot(origin, ray.direction);
    float     c            = glm::dot(origin, origin) - m_radius * m_radius;
    float     discriminant = b * b - 4.0f * a * c;
    // not hit by this ray
    float closestT = (-b - glm::sqrt(discriminant)) / (2.0f * a);
    if (closestT > 0.0f + 1e-4) { return closestT; }
    return std::nullopt;
}