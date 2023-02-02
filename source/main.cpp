#include "glm/fwd.hpp"
#include "renderer.h"

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

    scene.AddObject(std::make_shared<Sphere>(glm::vec3({0.0f, -100.5f, -1.0f}), 100.0f, 0));
    scene.AddObject(std::make_shared<Sphere>(glm::vec3({0.0f, 0.0f, -1.0f}), 0.5f, 1));
    scene.AddObject(std::make_shared<Sphere>(glm::vec3({-1.0f, 0.0f, -1.0f}), 0.5f, 2));
    scene.AddObject(std::make_shared<Sphere>(glm::vec3({1.0f, 0.0f, -1.0f}), 0.5f, 3));
}

int main(int argc, char** args) {
    // init
    constexpr float aspect = 16.0f / 9.0f;
    constexpr int imageHeight = 506, imageWidth = static_cast<int>(imageHeight * aspect);

    auto  renderer = Renderer(imageWidth, imageHeight);
    constexpr glm::vec3 cameraOrgin = {0.0f, 0.0f, 1.0f};
    constexpr glm::vec3 lookat = {0.0f, 0.0f, -1.0f};
    constexpr glm::vec3 updir = {0.0f, 1.0f, 0.0f};
    auto camera = std::make_unique<Camera>(cameraOrgin, lookat, updir, aspect, 90.0f);
    Scene activeScene;

    InitScene(activeScene);
    // bind scene and camera
    renderer.BindScene(activeScene);
    renderer.BindCamera(std::move(camera));
    // start render
    renderer.Render();
    return 0;
}