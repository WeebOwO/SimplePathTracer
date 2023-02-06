#include "bvh.h"
#include <optional>


bool BvhNode::BoundingBox(float t0, float t1, AABB& outputBox) const { 
    outputBox = box;
    return true;
}

bool BvhNode::Hit(const Ray& ray, float minTime, float maxTime, HitPayload& payload) const {
    if(!box.Hit(ray)) return false;
    auto hitLeft = left->Hit(ray, minTime, maxTime, payload);
    auto hitright = right->Hit(ray, minTime, maxTime, payload);
    return hitLeft || hitright;
}
