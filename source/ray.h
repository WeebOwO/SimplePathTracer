#pragma once

#include <glm/glm.hpp>
#include <optional>

struct Ray {
    glm::vec3 origin;
    glm::vec3 direction;
};

struct HitPayload {
    float     hitDistance;
    float     hitTime;
    glm::vec3 worldPos;
    glm::vec3 wordNormal;
    int       objectIndex;
};
