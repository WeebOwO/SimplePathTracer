#pragma once

#include <memory>
#include <vector>


#include <glm/glm.hpp>

#include "geometry.h"
#include "material.h"

struct Scene {
    std::vector<std::shared_ptr<HitableObject>> objects;
    std::vector<Material> materials;
    void AddObject(std::shared_ptr<HitableObject> object);
    void AddMaterial(const Material& material);
};

