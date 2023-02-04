#pragma once

#include <memory>
#include <optional>
#include <stdint.h>

#include "glm/fwd.hpp"
#include "ray.h"

struct AABB {
    AABB() = default;
    AABB(const AABB& other) = default;
    AABB& operator=(const AABB& other) = default;
    AABB(const glm::vec3& minPoint, const glm::vec3& maxPoint)
        : minPoint(minPoint), maxPoint(maxPoint) {}
    bool      Hit(const Ray& ray) const;
    glm::vec3 minPoint;
    glm::vec3 maxPoint;
};

struct HitableObject {
    virtual std::optional<float> Hit(const Ray& ray) const                              = 0;
    virtual uint32_t             GetMaterialIndex() const                               = 0;
    virtual glm::vec3            GetPositon() const                                     = 0;
    virtual bool                 BoundingBox(float t0, float t1, AABB& outputBox) const = 0;
};


class Sphere : public HitableObject {
public:
    Sphere(glm::vec3 positon, float radius, int index);
    std::optional<float> Hit(const Ray& ray) const override;
    uint32_t             GetMaterialIndex() const override { return m_materialIndex; }
    glm::vec3            GetPositon() const override { return m_position; }
    bool                 BoundingBox(float to, float t1, AABB& outputBox) const override;

private:
    glm::vec3 m_position{0.0f};
    float     m_radius{0.5f};
    uint32_t  m_materialIndex{0};
};


AABB SurroundingBox(const AABB& box1, const AABB& box2);

bool BoxCompare(const std::shared_ptr<HitableObject> a, const std::shared_ptr<HitableObject> b, int axis);

bool BoxCompareX(const std::shared_ptr<HitableObject> a, const std::shared_ptr<HitableObject> b);
bool BoxCompareY(const std::shared_ptr<HitableObject> a, const std::shared_ptr<HitableObject> b);
bool BoxCompareZ(const std::shared_ptr<HitableObject> a, const std::shared_ptr<HitableObject> b);
