#include "renderer.h"
#include "scene.h"

void InitScene(Scene& scene) {
    // material setting
    Material red;
    red.albedo = {1.0, 0.0f, 0.0f};
    // sphere setting
    Sphere sphere;
    sphere.position = {0.0f, 0.0f, 0.0f};
    sphere.radius = 0.5f;
    sphere.materialIndex = 0;
    // add sphere and material
    scene.materials.emplace_back(red);
    scene.spheres.emplace_back(sphere);
}

int main(int argc, char** args) {
    auto renderer = Renderer(900, 600);
    Scene activeScene;
    InitScene(activeScene);
    renderer.SetActiveScene(activeScene);
    renderer.Render();
    return 0;
}