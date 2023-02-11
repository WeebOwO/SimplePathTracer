#include "geometry.h"

#include <cmath>
#include <execution>
#include <iostream>
#include <limits>
#include <memory>
#include <stdint.h>

#include "misc.h"
#include "ray.h"

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

bool Sphere::Hit(const Ray& ray, float minTime, float maxTime, HitPayload& payload) const {
    glm::vec3 origin       = ray.origin - m_position;
    float     a            = glm::dot(ray.direction, ray.direction);
    float     b            = 2.0f * glm::dot(origin, ray.direction);
    float     c            = glm::dot(origin, origin) - m_radius * m_radius;
    float     discriminant = b * b - 4.0f * a * c;

    if (discriminant < 0) return false;
    float closestT = (-b - glm::sqrt(discriminant)) / (2.0f * a);
    if (closestT < minTime || closestT > maxTime) return false;

    payload.hitTime = closestT;

    payload.worldPos    = ray.origin + closestT * ray.direction;
    glm::vec3 outnormal = glm::normalize(payload.worldPos - m_position);
    payload.SetFaceNormal(ray, outnormal);

    return true;
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

bool XyRect::Hit(const Ray& ray, float minTime, float maxTime, HitPayload& payload) const {
    float t = (m_z - ray.origin.z) / ray.direction.z;
    if (t < minTime || t > maxTime) return false;

    float x = ray.origin.x + t * ray.direction.x;
    float y = ray.origin.y + t * ray.direction.y;

    if (x < m_xy[0] || y < m_xy[1] || x > m_xy[2] || y > m_xy[3]) return false;
    payload.SetFaceNormal(ray, glm::vec3(0, 0, 1));
    payload.hitTime  = t;
    payload.worldPos = ray.origin + ray.direction * t;
    return true;
}

bool XyRect::BoundingBox(float to, float t1, AABB& outputBox) const {
    outputBox =
        AABB(glm::vec3(m_xy[0], m_xy[1], m_z - 1e-4), glm::vec3(m_xy[2], m_xy[3], m_z + 1e-4));
    return true;
}

bool XzRect::Hit(const Ray& ray, float minTime, float maxTime, HitPayload& payload) const {
    float t = (m_y - ray.origin.y) / ray.direction.y;
    if (t < minTime || t > maxTime) return false;

    float x = ray.origin.x + t * ray.direction.x;
    float z = ray.origin.z + t * ray.direction.z;

    if (x < m_xz[0] || z < m_xz[1] || x > m_xz[2] || z > m_xz[3]) return false;
    payload.SetFaceNormal(ray, glm::vec3(0, 1, 0));
    payload.hitTime  = t;
    payload.worldPos = ray.origin + ray.direction * t;
    return true;
}

bool XzRect::BoundingBox(float to, float t1, AABB& outputBox) const {
    outputBox =
        AABB(glm::vec3(m_xz[0], m_y - 1e-4, m_xz[1]), glm::vec3(m_xz[2], m_y + 1e-4, m_xz[3]));
    return true;
}

bool YzRect::Hit(const Ray& ray, float minTime, float maxTime, HitPayload& payload) const {
    float t = (m_x - ray.origin.x) / ray.direction.x;
    if (t < minTime || t > maxTime) return false;

    float y = ray.origin.y + t * ray.direction.y;
    float z = ray.origin.z + t * ray.direction.z;

    if (y < m_yz[0] || z < m_yz[1] || y > m_yz[2] || z > m_yz[3]) return false;
    payload.SetFaceNormal(ray, glm::vec3(1, 0, 0));
    payload.hitTime  = t;
    payload.worldPos = ray.origin + ray.direction * t;
    return true;
}

bool YzRect::BoundingBox(float to, float t1, AABB& outputBox) const {
    outputBox =
        AABB(glm::vec3(m_x - 1e-4, m_yz[0], m_yz[1]), glm::vec3(m_x + 1e-4, m_yz[2], m_yz[3]));
    return true;
}

Box::Box(const glm::vec3& minPoint, const glm::vec3& maxPoint, uint32_t materialIndex)
    : HitableObject(materialIndex), m_minPoint(minPoint), m_maxPoint(maxPoint) {
    m_sides[0] = std::make_shared<XyRect>(glm::vec4(minPoint.x, minPoint.y, maxPoint.x, maxPoint.y),
                                          minPoint.z, materialIndex);
    m_sides[1] = std::make_shared<XyRect>(glm::vec4(minPoint.x, minPoint.y, maxPoint.x, maxPoint.y),
                                          maxPoint.z, materialIndex);

    m_sides[2] = std::make_shared<XzRect>(glm::vec4(minPoint.x, minPoint.z, maxPoint.x, maxPoint.z),
                                          minPoint.y, materialIndex);
    m_sides[3] = std::make_shared<XzRect>(glm::vec4(minPoint.x, minPoint.z, maxPoint.x, maxPoint.z),
                                          maxPoint.y, materialIndex);

    m_sides[4] = std::make_shared<YzRect>(glm::vec4(minPoint.y, minPoint.z, maxPoint.y, maxPoint.z),
                                          minPoint.x, materialIndex);
    m_sides[5] = std::make_shared<YzRect>(glm::vec4(minPoint.y, minPoint.z, maxPoint.y, maxPoint.z),
                                          maxPoint.x, materialIndex);
}

bool Box::Hit(const Ray& ray, float minTime, float maxTime, HitPayload& payload) const {
    float closestHitTime = maxTime;
    bool  hitAny         = false;
    for (uint32_t i = 0; i < m_sides.size(); ++i) {
        const auto& hitObejct = m_sides[i];
        if (hitObejct->Hit(ray, 0.0001, closestHitTime, payload)) {
            closestHitTime = payload.hitTime;
            hitAny         = true;
        }
    }
    return hitAny;
}

bool Box::BoundingBox(float to, float t1, AABB& outputBox) const {
    outputBox = AABB(m_minPoint, m_maxPoint);
    return true;
}

bool RotateY::Hit(const Ray& ray, float minTime, float maxTime, HitPayload& payload) const {
    glm::vec3 origin    = ray.origin;
    glm::vec3 direction = ray.direction;

    origin.x = m_cos * ray.origin.x - m_sin * ray.origin.z;
    origin.z = m_sin * ray.origin.x + m_cos * ray.origin.z;

    direction.x = m_cos * ray.direction.x - m_sin * ray.direction.z;
    direction.z = m_sin * ray.direction.x + m_cos * ray.direction.z;

    Ray rotated{origin, direction};

    if (!m_ptr->Hit(rotated, minTime, maxTime, payload)) return false;

    auto p      = payload.worldPos;
    auto normal = payload.wordNormal;

    p.x = m_cos * payload.worldPos.x + m_sin * payload.worldPos.z;
    p.z = -m_sin * payload.worldPos.x + m_cos * payload.worldPos.z;

    normal.x = m_cos * payload.wordNormal.x + m_sin * payload.wordNormal.z;
    normal.z = -m_sin * payload.wordNormal.x + m_cos * payload.wordNormal.z;

    payload.worldPos = p;
    payload.SetFaceNormal(rotated, normal);

    return true;
}

bool Translate::Hit(const Ray& ray, float minTime, float maxTime, HitPayload& payload) const {
    Ray movedRay{ray.origin - m_offset, ray.direction};
    if(!m_ptr->Hit(movedRay, minTime, maxTime, payload)) return false;

    payload.worldPos += m_offset;
    payload.SetFaceNormal(movedRay, payload.wordNormal);

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