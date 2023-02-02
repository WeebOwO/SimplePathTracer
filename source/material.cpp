#include "material.h"
#include "misc.h"

namespace pbr {
Ray Scatter(const Material& material, const Ray& IncomingRay, const HitPayload& payload) {
    Ray scatterRay;
    scatterRay.origin = payload.worldPos;
    scatterRay.direction =
        glm::reflect(IncomingRay.direction,
                     payload.wordNormal + material.roughness * pbr::RandomHemiSphereDir());
    return scatterRay;
}
} // namespace pbr
