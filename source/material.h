#pragma once

#include "glm/fwd.hpp"
#include "material.h"
#include "ray.h"

enum class MaterialType : uint8_t { object, light };

struct Material {
    glm::vec3    albedo;
    float        roughness;
    float        metallic;
    float        f0;
    glm::vec3    emit;
    MaterialType type = MaterialType::object;
};

namespace pbr {
Ray Scatter(const Material& material, const Ray& IncomingRay, const HitPayload& payload);

float SchlickApproximation(float cos, const glm::vec3& f0);
} // namespace pbr
