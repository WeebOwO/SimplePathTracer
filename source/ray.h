#pragma once

#include "glm/fwd.hpp"
#include <glm/glm.hpp>

struct Ray {
    glm::vec3 origin;
    glm::vec3 direction;
};

struct HitPayload {
    bool      frontFace;
    int       objectIndex;
    float     hitDistance;
    float     hitTime;
    glm::vec3 worldPos;
    glm::vec3 wordNormal;

    void SetFaceNormal(const Ray& ray, const glm::vec3& outwardNormal) {
        frontFace  = glm::dot(ray.direction, outwardNormal) < 0;
        wordNormal = frontFace ? outwardNormal : -outwardNormal;
    }
};
