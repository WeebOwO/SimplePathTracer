#include "scene.h"
#include "ray.h"
#include <stdint.h>

Material Scene::GetMaterial(uint32_t objectIndex) const{
    uint32_t materialIndex = m_objects[objectIndex]->GetMaterialIndex();
    return m_materials[materialIndex];
}

HitPayload Scene::Hit(const Ray &ray) const {
    HitPayload payload;
    float      closestHitTime = std::numeric_limits<float>::max();
    payload.objectIndex       = -1;

    for (size_t i = 0; i < m_objects.size(); ++i) {
        const auto& hitObejct = m_objects[i];
        if (auto hitTime = hitObejct->Hit(ray)) {
            if (hitTime < closestHitTime) {
                closestHitTime      = hitTime.value();
                payload.objectIndex = i;
            }
        }
    }

    // if hit something
    if (payload.objectIndex != -1) {
        const auto& object = m_objects[payload.objectIndex];
        payload.hitTime    = closestHitTime;
        payload.worldPos   = ray.origin + ray.direction * closestHitTime;
        payload.wordNormal = glm::normalize(payload.worldPos - object->GetPositon());
    }

    return payload;
}

void Scene::AddObject(std::shared_ptr<HitableObject> object) { m_objects.emplace_back(object); }

void Scene::AddMaterial(const Material& material) { m_materials.emplace_back(material); }