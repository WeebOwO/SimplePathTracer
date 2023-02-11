#pragma once

#include "geometry.h"

#include <array>
#include <random>
#include <stdint.h>

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
    static thread_local std::mt19937               m_randomEngine;
    static std::uniform_int_distribution<uint32_t> m_distribution;
};


struct Onb {
    glm::vec3 Local(float a, float b, float c) {
       return u * a + b * v + c * w;
    }
    glm::vec3 Local(const glm::vec3& factor) const {
        return factor[0] * u + factor[1] * v + factor[2] * w;
    }
    void BuildFromW(const glm::vec3& normal) {
        w = glm::normalize(normal);
        glm::vec3 a = (fabs(w.x) > 0.9f) ? glm::vec3(0, 1, 0) : glm::vec3(1, 0, 0);
        v = glm::normalize(glm::cross(w, a));
        u = glm::cross(w, v);
    }
    glm::vec3 u, v, w;
};

namespace misc {
float DegreeToRadians(float degree);
float RandomFloat();
float RandomFloat(float min, float max);
}

namespace pbr {
glm::vec3 RandomUnitSphereDir();
glm::vec3 RandomHemiSphereDir();
glm::vec3 RandomCosineDirection();
constexpr float pi = 3.1415926535;
} // namespace pbr