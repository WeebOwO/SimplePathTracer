#include "geometry.h"
#include "glm/fwd.hpp"

bool Sphere::BoundingBox(float to, float t1, AABB &outputBox) const{
    auto range = glm::vec3(m_radius, m_radius, m_radius);
    outputBox = AABB(m_position - range, m_position + range);
    return true;
}

bool AABB::Hit(const Ray &ray, float tmin, float tmax) const {
    for(uint32_t i = 0; i < 3; ++i) {
        float inverseSpeed = 1.0f / ray.direction[i];
        float t0 = (minPoint[i] - ray.origin[i]) * inverseSpeed;
        float t1 = (maxPoint[i] - ray.origin[i]) * inverseSpeed;
        if(inverseSpeed < 0.0f) std::swap(t0, t1);
        tmin = t0 > tmin ? t0 : tmin;
        tmax = t1 > tmax ? tmax : t1; 
        if(tmin > tmax) return false;
    }
    return true;
}

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
    if (closestT > 0.0f + 1e-4) return closestT; 
    return std::nullopt;
}