#include "misc.h"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include <random>

std::mt19937 thread_local Random::m_randomEngine;
std::uniform_int_distribution<std::mt19937::result_type> Random::m_distribution;

namespace pbr {
glm::vec3 RandomUnitSphereDir() {
    while (true) {
        auto dir = Random::Vec3(-1.0f, 1.0f);
        if (glm::length(dir) > 1.0f) continue;
        ;
        return glm::normalize(dir);
    }
}
glm::vec3 RandomHemiSphereDir() {
    while (true) {
        auto dir = glm::vec3(Random::Float(), Random::Float(), Random::Float());
        if (glm::length(dir) > 1.0f) continue;
        ;
        return glm::normalize(dir);
    }
}
} // namespace pbr