#include "material.h"

namespace pbr {
float Reflectance(float cosine, float refIndex) {
    // Use Schlick's approximation for reflectance.
    auto r0 = (1 - refIndex) / (1 + refIndex);
    r0      = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
}
} // namespace pbr