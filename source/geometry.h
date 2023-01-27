#pragma once

#include "ray.h"
#include <glm/glm.hpp>

struct HitableObject {
    virtual std::optional<HitPayload> Hit(const Ray& ray) = 0;
};

class Sphere : public HitableObject{
public:
    Sphere(glm::vec3 positon, float radius, int index) noexcept;
    virtual std::optional<HitPayload> Hit(const Ray& ray) override;
private:
    // public data member
    glm::vec3 m_position{0.0f};
    float     m_radius{0.5f};
    int       m_materialIndex{0};
};
