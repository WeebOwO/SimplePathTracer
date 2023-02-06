#pragma once

#include "glm/fwd.hpp"
#include "material.h"
#include "ray.h"

enum class MaterialType : uint8_t { light, diffuse, metal };

struct Material {
    glm::vec3    albedo;
    glm::vec3    emit;
    float        fuzz;
    MaterialType type = MaterialType::diffuse;
};

namespace pbr {
bool Scatter(const Material& material, const Ray& IncomingRay, const HitPayload& payload, Ray& scatter);

float SchlickApproximation(float cos, const glm::vec3& f0);
} // namespace pbr
