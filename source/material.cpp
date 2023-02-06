#include "material.h"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include "misc.h"
#include "ray.h"

namespace pbr {
bool Scatter(const Material& material, const Ray& IncomingRay, const HitPayload& payload, Ray& scatter) {
    if(material.type == MaterialType::diffuse) {
        glm::vec3 dirtection = payload.wordNormal + pbr::RandomUnitSphereDir();
        scatter.origin = payload.worldPos;
        scatter.direction = dirtection;
        return true;
    }
    if(material.type == MaterialType::metal) {
        scatter.origin = payload.worldPos;
        scatter.direction = glm::reflect(IncomingRay.direction, payload.wordNormal) + material.fuzz * RandomUnitSphereDir();
        return glm::dot(scatter.direction, payload.wordNormal) > 0.0f;
    }
    return false;
}
} // namespace pbr
