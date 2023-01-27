#pragma once

#include <glm/glm.hpp>

struct Material {
    glm::vec3 albedo{1.0f};
    float     roughness{1.0f};
    float     metallic{0.0f};
};

