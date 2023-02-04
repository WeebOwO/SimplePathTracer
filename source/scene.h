#pragma once

#include <memory>
#include <vector>

#include "bvh.h"
#include "geometry.h"
#include "material.h"
#include "ray.h"


struct Light {
    
};

class Scene {
public:
    void AddObject(std::shared_ptr<HitableObject> object) {m_objects.emplace_back(object);}
    void AddMaterial(const Material& material) {m_materials.emplace_back(material);}
    auto& GetObjects() const {return m_objects;}
    const Material& GetMaterial(uint32_t objectIndex) const;

    HitPayload Hit(const Ray& ray) const;
    bool BoundingBox(float time0, float time1, AABB& output) const;
    std::shared_ptr<BvhNode> BuildBVH(int left, int right);
private:    
    std::vector<std::shared_ptr<HitableObject>> m_objects;
    std::vector<Material> m_materials;
    std::shared_ptr<BvhNode> m_root;
    
};

