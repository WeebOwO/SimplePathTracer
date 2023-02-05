#pragma once

#include <memory>
#include <optional>
#include <stdint.h>

#include "glm/fwd.hpp"
#include "ray.h"

struct AABB {
    AABB()                             = default;
    AABB(const AABB& other)            = default;
    AABB& operator=(const AABB& other) = default;
    AABB(const glm::vec3& minPoint, const glm::vec3& maxPoint)
        : minPoint(minPoint), maxPoint(maxPoint) {}
    bool      Hit(const Ray& ray) const;
    glm::vec3 minPoint;
    glm::vec3 maxPoint;
};

struct HitableObject {
    constexpr HitableObject(uint32_t materialIndex) : m_materialIndex(materialIndex) {}
    virtual std::optional<float> Hit(const Ray& ray) const                              = 0;
    virtual void                 HitSolve(const Ray& ray, HitPayload& payload) const    = 0;
    virtual bool                 BoundingBox(float t0, float t1, AABB& outputBox) const = 0;
    uint32_t                     GetMaterialIndex() const { return m_materialIndex; }
    uint32_t                     m_materialIndex{0};
};

class Sphere : public HitableObject {
public:
    constexpr Sphere(glm::vec3 positon, float radius, uint32_t index)
        : HitableObject(index), m_position(positon), m_radius(radius) {}
    virtual std::optional<float> Hit(const Ray& ray) const override;
    virtual void                 HitSolve(const Ray& ray, HitPayload& payload) const override;
    virtual bool                 BoundingBox(float to, float t1, AABB& outputBox) const override;

private:
    glm::vec3 m_position{0.0f};
    float     m_radius{0.5f};
};

class XyRect : public HitableObject {
public:
    constexpr XyRect(const glm::vec4& xy, float z, uint32_t index)
        : HitableObject(index), m_xy(xy), m_z(z) {}
    virtual std::optional<float> Hit(const Ray& ray) const override;
    virtual void                 HitSolve(const Ray& ray, HitPayload& payload) const override;
    virtual bool                 BoundingBox(float to, float t1, AABB& outputBox) const override;

private:
    glm::vec4 m_xy; // xmin, ymin, xmax, ymax
    float     m_z;
};


AABB SurroundingBox(const AABB& box1, const AABB& box2);

bool BoxCompare(const std::shared_ptr<HitableObject> a, const std::shared_ptr<HitableObject> b,
                int axis);

bool BoxCompareX(const std::shared_ptr<HitableObject> a, const std::shared_ptr<HitableObject> b);
bool BoxCompareY(const std::shared_ptr<HitableObject> a, const std::shared_ptr<HitableObject> b);
bool BoxCompareZ(const std::shared_ptr<HitableObject> a, const std::shared_ptr<HitableObject> b);
