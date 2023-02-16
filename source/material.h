#pragma once

#include "glm/fwd.hpp"
#include "material.h"
#include "ray.h"

enum class MaterialType : uint8_t { Light, Diffuse, Metal, Dielectric };

constexpr glm::vec3 blue     = glm::vec3(0.5f, 0.7f, 1.0f);
constexpr glm::vec3 white    = glm::vec3(1.0f, 1.0f, 1.0f);
constexpr glm::vec3 black    = glm::vec3(0.0f, 0.0f, 0.0f);

struct Material {
    glm::vec3    albedo;
    glm::vec3    emit;
    float        fuzz;
    MaterialType type = MaterialType::Diffuse;
};

namespace pbr {
float Reflectance(float cosine, float refIndex);
}