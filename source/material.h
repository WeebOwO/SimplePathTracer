#pragma once

#include <glm/glm.hpp>

#include "glm/fwd.hpp"
#include "ray.h"

struct Material {
    glm::vec3 albedo;
    float     roughness;
    float     metallic;
};

namespace pbr {}
