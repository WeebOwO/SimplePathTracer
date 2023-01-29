#pragma once

#include "glm/fwd.hpp"
#include <glm/glm.hpp>
#include <random>

class Random {
public:
    static void Init() { m_randomEngine.seed(std::random_device()()); }

    static uint32_t UInt() { return m_distribution(m_randomEngine); }

    static uint32_t UInt(uint32_t min, uint32_t max) {
        return min + (m_distribution(m_randomEngine) % (max - min + 1));
    }

    static float Float() {
        return (float)m_distribution(m_randomEngine) / (float)std::numeric_limits<uint32_t>::max();
    }

    static glm::vec3 Vec3() { return glm::vec3(Float(), Float(), Float()); }

    static glm::vec3 Vec3(float min, float max) {
        return glm::vec3(Float() * (max - min) + min, Float() * (max - min) + min,
                         Float() * (max - min) + min);
    }

    static glm::vec3 InUnitSphere() { return glm::normalize(Vec3(-1.0f, 1.0f)); }

private:
    static thread_local std::mt19937                                m_randomEngine;
    static std::uniform_int_distribution<std::mt19937::result_type> m_distribution;
};

namespace pbr {
glm::vec3 RandomUnitSphereDir();
glm::vec3 RandomHemiSphereDir();
} // namespace pbr