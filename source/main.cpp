#include "glm/fwd.hpp"
#include "material.h"
#include "misc.h"
#include "renderer.h"
#include <iostream>
#include <memory>
#include <ranges>

void InitScene(Scene& scene) {
    // material setting
    Material material0{glm::vec3(0.8f, 0.8f, 0.0f), 1.0f, 0.0f};
    Material material1{glm::vec3(0.7f, 0.3f, 0.3f), 1.0f, 0.0f};
    Material material2{glm::vec3(0.8f, 0.8f, 0.8f), 0.15f, 0.0f};
    Material material3{glm::vec3(0.8f, 0.6f, 0.2f), 0.15f, 0.0f};

    scene.AddMaterial(material0);
    scene.AddMaterial(material1);
    scene.AddMaterial(material2);
    scene.AddMaterial(material3);

    scene.AddObject(std::make_shared<Sphere>(glm::vec3({0.0f, -100.5f, -2.0f}), 100.0f, 0));
    scene.AddObject(std::make_shared<Sphere>(glm::vec3({0.0f, 0.0f, -2.0f}), 0.5f, 1));
    scene.AddObject(std::make_shared<Sphere>(glm::vec3({-1.0f, 0.0f, -2.0f}), 0.5f, 2));
    scene.AddObject(std::make_shared<Sphere>(glm::vec3({1.0f, 0.0f, -2.0f}), 0.5f, 3));
}

int main(int argc, char** args) {
    // init
    auto  renderer = Renderer(900, 506);
    Scene activeScene;
    InitScene(activeScene);
    // render start
    renderer.SetActiveScene(activeScene);
    renderer.Render();
    return 0;
}