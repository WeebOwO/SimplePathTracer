#pragma once

#include <memory>
#include <vector>

#include "geometry.h"

// BvhNode define
struct BvhNode : public HitableObject {
    std::optional<float> Hit(const Ray& ray) const override;
    bool                 BoundingBox(float t0, float t1, AABB& outputBox) const override;
    
    std::shared_ptr<HitableObject> left {nullptr};
    std::shared_ptr<HitableObject> right {nullptr};
    AABB                           box;
};