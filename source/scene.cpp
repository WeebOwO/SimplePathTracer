#include "scene.h"

void Scene::AddObject(std::shared_ptr<HitableObject> object) {
    objects.emplace_back(object);
}

void Scene::AddMaterial(const Material& material) {
    materials.emplace_back(material);
}