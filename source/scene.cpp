#include "scene.h"
#include "bvh.h"
#include "geometry.h"
#include "misc.h"
#include "ray.h"
#include <iostream>
#include <memory>
#include <stdint.h>

const Material& Scene::GetMaterial(uint32_t objectIndex) const {
    uint32_t materialIndex = m_objects[objectIndex]->GetMaterialIndex();
    return m_materials[materialIndex];
}

HitPayload Scene::Hit(const Ray& ray) const {
    HitPayload payload;
    float      closestHitTime = std::numeric_limits<float>::max();
    payload.objectIndex       = -1;
    bool hitAny = false;
    for (uint32_t i = 0; i < m_objects.size(); ++i) {
        const auto& hitObejct = m_objects[i];
        if (hitObejct->Hit(ray, 0.0001, closestHitTime, payload)) {
            payload.objectIndex = i;
            closestHitTime = payload.hitTime;
            hitAny = true;
        }
    }
    return payload;
}

bool Scene::BoundingBox(float time0, float time1, AABB& output) const {
    if (m_objects.empty()) return false;
    AABB temp;
    bool firstBox = true;
    // get the biggest box that contains all objects
    for (const auto& object : m_objects) {
        if (!object->BoundingBox(time0, time1, temp)) return false;
        output   = firstBox ? temp : SurroundingBox(output, temp);
        firstBox = false;
    }
    return true;
}

std::shared_ptr<BvhNode> Scene::BuildBVH(int start, int end) {
    int  axis        = Random::UInt() % 3;
    auto compareFunc = (axis == 0) ? BoxCompareX : (axis == 1) ? BoxCompareY : BoxCompareZ;

    std::unique_ptr<BvhNode> root;
    int len = end - start; // left open, right close
    if(len == 1) {
        root->left = m_objects[start];
        root->right = m_objects[end];
    }
    else if(len == 2) {
        if(compareFunc(m_objects[start], m_objects[start + 1])) {
            root->left = m_objects[start];
            root->right = m_objects[start + 1];
        } else {
            root->left = m_objects[start + 1];
            root->right = m_objects[start];
        }
    }
    else {
        std::sort(m_objects.begin() + start, m_objects.begin() + end, compareFunc);
        int mid = start + len / 2;
        root->left = BuildBVH(start, mid);
        root->right = BuildBVH(mid, end);
    }
    AABB leftBox, rightBox;
    auto left = root->left, right = root->right;
    if(!left->BoundingBox(0, 0, leftBox) || !right->BoundingBox(0, 0, rightBox)) {
        std::cerr << "Bounding box failed!\n";
    }
    root->box = SurroundingBox(leftBox, rightBox);
    return root;
}
