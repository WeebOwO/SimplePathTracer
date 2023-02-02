#pragma once

#include <optional>
#include <stdint.h>

#include "geometry.h"
#include "glm/fwd.hpp"
#include "ray.h"

struct AABB {
    AABB(const glm::vec3& minPoint, const glm::vec3& maxPoint)
        : minPoint(minPoint), maxPoint(maxPoint) {}
    bool      Hit(const Ray& ray, float tmin, float tmax) const;
    glm::vec3 minPoint;
    glm::vec3 maxPoint;
};

struct HitableObject {
    virtual std::optional<float> Hit(const Ray& ray)                                    = 0;
    virtual uint32_t             GetMaterialIndex() const                               = 0;
    virtual glm::vec3            GetPositon() const                                     = 0;
    virtual bool                 BoundingBox(float t0, float t1, AABB& outputBox) const = 0;
};

class Sphere : public HitableObject {
public:
    Sphere(glm::vec3 positon, float radius, int index);
    std::optional<float> Hit(const Ray& ray) override;
    uint32_t             GetMaterialIndex() const override { return m_materialIndex; }
    glm::vec3            GetPositon() const override { return m_position; }
    bool                 BoundingBox(float to, float t1, AABB& outputBox) const override;

private:
    glm::vec3 m_position{0.0f};
    float     m_radius{0.5f};
    uint32_t  m_materialIndex{0};
};
