#pragma once

#include <memory>
#include <vector>

#include "geometry.h"
#include "material.h"
#include "ray.h"

class Scene {
public:
    void AddObject(std::shared_ptr<HitableObject> object);
    void AddMaterial(const Material& material);
    Material GetMaterial(uint32_t objectIndex) const;
    HitPayload Hit(const Ray& ray) const;
private:
    std::vector<std::shared_ptr<HitableObject>> m_objects;
    std::vector<Material> m_materials;
};

