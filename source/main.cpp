#include "geometry.h"
#include "glm/fwd.hpp"
#include "material.h"
#include "renderer.h"
#include "scene.h"
#include <memory>

void InitScene(Scene& scene) {
    Material red;
    red.albedo = {1.0f, 0.0f, 0.0f};
    scene.AddObject(std::make_shared<Sphere>(glm::vec3({0.0f, 0.0f, -2.0f}), 0.5f, 0));
    scene.AddMaterial(red);
}

int main(int argc, char** args) {
    // // init
    auto  renderer = Renderer(900, 600);
    Scene activeScene;
    InitScene(activeScene);
    // render start
    renderer.SetActiveScene(activeScene);
    renderer.Render();
    return 0;
}