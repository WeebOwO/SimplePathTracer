#include "misc.h"
#include <random>

#include "geometry.h"


std::mt19937 thread_local Random::m_randomEngine;
std::uniform_int_distribution<uint32_t> Random::m_distribution;

namespace misc {
float RandomFloat() {
    return rand() / (RAND_MAX + 1.0);
}
float RandomFloat(float min, float max) {
    return min + (max - min) * RandomFloat();
}
float DegreeToRadians(float degree) { return degree / 180.0f * pbr::pi; }
} // namespace misc

namespace pbr {
glm::vec3 RandomUnitSphereDir() {
    while (true) {
        auto dir = Random::Vec3(-1.0f, 1.0f);
        if (glm::length(dir) > 1.0f) continue;
        return glm::normalize(dir);
    }
}

glm::vec3 RandomHemiSphereDir() {
    while (true) {
        auto dir = glm::vec3(Random::Float(), Random::Float(), Random::Float());
        if (glm::length(dir) > 1.0f) continue;
        return glm::normalize(dir);
    }
}

glm::vec3 RandomCosineDirection() {
    float r1 = Random::Float();
    float r2 = Random::Float();
    float z = sqrt(1 - r2);

    float phi = 2 * pbr::pi * r1;
    float x = cos(phi) * sqrt(r2);
    float y = sin(phi) * sqrt(r2);

    return glm::vec3(x, y, z);
}

} // namespace pbr