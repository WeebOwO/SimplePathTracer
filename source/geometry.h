#pragma once

#include "glm/fwd.hpp"
#include "ray.h"
#include <glm/glm.hpp>
#include <stdint.h>

struct HitableObject {
    virtual std::optional<float> Hit(const Ray& ray)      = 0;
    virtual uint32_t             GetMaterialIndex() const = 0;
    virtual glm::vec3            GetPositon() const       = 0;
};

class Sphere : public HitableObject {
public:
    Sphere(glm::vec3 positon, float radius, int index);
    virtual std::optional<float> Hit(const Ray& ray) override;
    virtual uint32_t             GetMaterialIndex() const override { return m_materialIndex; }
    virtual glm::vec3            GetPositon() const override { return m_position; }

private:
    glm::vec3 m_position{0.0f};
    float     m_radius{0.5f};
    uint32_t  m_materialIndex{0};
};
