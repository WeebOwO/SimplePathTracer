#pragma once

#include <memory>
#include <vector>

#include "geometry.h"

// BvhNode define
struct BvhNode : public HitableObject {
    virtual bool Hit(const Ray& ray, float minTime, float maxTime, HitPayload& payload) const override;
    bool                 BoundingBox(float t0, float t1, AABB& outputBox) const override;
    
    std::shared_ptr<HitableObject> left {nullptr};
    std::shared_ptr<HitableObject> right {nullptr};
    AABB                           box;
};