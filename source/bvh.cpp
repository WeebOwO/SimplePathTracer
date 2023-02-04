#include "bvh.h"
#include <optional>


bool BvhNode::BoundingBox(float t0, float t1, AABB& outputBox) const { 
    outputBox = box;
    return true;
}

std::optional<float> BvhNode::Hit(const Ray &ray) const {
    if(!box.Hit(ray)) return false;
    auto hitLeft = left->Hit(ray);
    auto hitright = right->Hit(ray);
    return hitLeft || hitright;
}
