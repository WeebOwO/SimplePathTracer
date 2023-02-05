#include "geometry.h"
#include "glm/fwd.hpp"

#include <iostream>
#include <optional>
#include <stdint.h>

bool Sphere::BoundingBox(float to, float t1, AABB& outputBox) const {
    auto range = glm::vec3(m_radius, m_radius, m_radius);
    outputBox  = AABB(m_position - range, m_position + range);
    return true;
}

bool AABB::Hit(const Ray& ray) const {
    float tmin = 0.0f, tmax = std::numeric_limits<float>::max();
    for (uint32_t i = 0; i < 3; ++i) {
        float inverseSpeed = 1.0f / ray.direction[i];
        float t0           = (minPoint[i] - ray.origin[i]) * inverseSpeed;
        float t1           = (maxPoint[i] - ray.origin[i]) * inverseSpeed;
        if (inverseSpeed < 0.0f) std::swap(t0, t1);
        tmin = t0 > tmin ? t0 : tmin;
        tmax = t1 > tmax ? tmax : t1;
        if (tmin > tmax) return false;
    }
    return true;
}

std::optional<float> Sphere::Hit(const Ray& ray) const {
    glm::vec3 origin       = ray.origin - m_position;
    float     a            = glm::dot(ray.direction, ray.direction);
    float     b            = 2.0f * glm::dot(origin, ray.direction);
    float     c            = glm::dot(origin, origin) - m_radius * m_radius;
    float     discriminant = b * b - 4.0f * a * c;
    if (discriminant < 0) return std::nullopt;
    float closestT = (-b - glm::sqrt(discriminant)) / (2.0f * a);
    if (closestT > 0.0f + 1e-4) return closestT;
    return std::nullopt;
}

void Sphere::HitSolve(const Ray& ray, HitPayload& payload) const {
    payload.worldPos   = ray.origin + ray.direction * payload.hitTime;
    payload.wordNormal = glm::normalize(payload.worldPos - m_position);
}

AABB SurroundingBox(const AABB& box1, const AABB& box2) {
    glm::vec3 minPoint = {
        std::min(box1.minPoint.x, box2.minPoint.x),
        std::min(box1.minPoint.y, box2.minPoint.y),
        std::min(box1.minPoint.z, box2.minPoint.x),
    };

    glm::vec3 maxPoint = {
        std::max(box1.maxPoint.x, box2.maxPoint.x),
        std::max(box1.maxPoint.y, box2.maxPoint.y),
        std::max(box1.maxPoint.z, box2.maxPoint.x),
    };

    return AABB(minPoint, maxPoint);
}

std::optional<float> XyRect::Hit(const Ray& ray) const { 
    float t = (m_z - ray.origin.z) / ray.direction.z;
    float x = ray.origin.x + t * ray.direction.x;
    float y = ray.origin.y + t * ray.direction.y;
    if(x < m_xy[0] || y < m_xy[1] || x > m_xy[2] || y > m_xy[3]) return std::nullopt;
    return t;
}

void XyRect::HitSolve(const Ray& ray, HitPayload& payload) const {
    glm::vec3 wordNormal = glm::vec3(0, 0, -1);
    payload.SetFaceNormal(ray, wordNormal);
    payload.worldPos = ray.origin + ray.direction * payload.hitTime;
}

bool XyRect::BoundingBox(float to, float t1, AABB& outputBox) const {
    outputBox =
        AABB(glm::vec3(m_xy[0], m_xy[1], m_z - 1e-4), glm::vec3(m_xy[2], m_xy[3], m_z + 1e-4));
    return true;
}

bool BoxCompare(const std::shared_ptr<HitableObject> a, const std::shared_ptr<HitableObject> b,
                int axis) {
    AABB boxa, boxb;
    if (!a->BoundingBox(0, 0, boxa) || !b->BoundingBox(0, 0, boxb)) {
        std::cerr << "No bounding box created\n";
    }
    return boxa.minPoint[axis] < boxb.minPoint[axis];
}

bool BoxCompareX(const std::shared_ptr<HitableObject> a, const std::shared_ptr<HitableObject> b) {
    return BoxCompare(a, b, 0);
}
bool BoxCompareY(const std::shared_ptr<HitableObject> a, const std::shared_ptr<HitableObject> b) {
    return BoxCompare(a, b, 1);
}
bool BoxCompareZ(const std::shared_ptr<HitableObject> a, const std::shared_ptr<HitableObject> b) {
    return BoxCompare(a, b, 2);
}