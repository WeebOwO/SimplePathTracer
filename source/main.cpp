#include "geometry.h"
#include "glm/fwd.hpp"
#include "material.h"
#include "renderer.h"
#include "misc.h"
#include "scene.h"
#include <memory>

static constexpr glm::vec3 cameraOrgin = {278, 278, -800};
static constexpr glm::vec3 lookat      = {278, 278, 0};
static constexpr glm::vec3 updir       = {0.0f, 1.0f, 0.0f};
static constexpr float     verticalFov = 40.0f;


Scene InitScene(uint32_t index) {
    Scene scene;
    if(index == 0) {
        Material material0{glm::vec3(0.8f, 0.8f, 0.0f), {0.0f, 0.0f, 0.0f}, 1.0f, MaterialType::diffuse};
        Material material1{glm::vec3(0.7f, 0.3f, 0.3f), {0.0f, 0.0f, 0.0f}, 1.0f, MaterialType::diffuse};
        Material material2{glm::vec3(0.8f, 0.8f, 0.8f), {0.0f, 0.0f, 0.0f}, 0.1f, MaterialType::metal};
        Material material3{glm::vec3(0.8f, 0.6f, 0.2f), {0.0f, 0.0f, 0.0f}, 1.0f, MaterialType::metal};

        scene.AddMaterial(material0);
        scene.AddMaterial(material1);
        scene.AddMaterial(material2);
        scene.AddMaterial(material3);

        scene.AddObject(std::make_shared<Sphere>(glm::vec3({0.0f, -100.5f, -1.0f}), 100.0f, 0));
        scene.AddObject(std::make_shared<Sphere>(glm::vec3({0.0f, 0.0f, -1.0f}), 0.5f, 1));
        scene.AddObject(std::make_shared<Sphere>(glm::vec3({-1.0f, 0.0f, -1.0f}), 0.5f, 2));
        scene.AddObject(std::make_shared<Sphere>(glm::vec3({1.0f, 0.0f, -1.0f}), 0.5f, 3));
    }
    else if(index == 1) {
        Material red = {glm::vec3(.65f, .05f, .05f), black, 0, MaterialType::diffuse};
        Material white  = {glm::vec3(.73, .73, .73), black, 0, MaterialType::diffuse};
        Material green  = {glm::vec3(.12, .45, .15), black, 0, MaterialType::diffuse};
        Material light  = {{1.0f, 1.0f, 1.0f}, {15.0f, 15.0f, 15.0f}, 0, MaterialType::light};

        scene.AddMaterial(red);
        scene.AddMaterial(white);
        scene.AddMaterial(green);
        scene.AddMaterial(light);

        scene.AddObject(std::make_shared<YzRect>(glm::vec4(0, 0, 555, 555), 555, 2));
        scene.AddObject(std::make_shared<YzRect>(glm::vec4(0, 0, 555, 555), 0, 0));
        scene.AddObject(std::make_shared<XzRect>(glm::vec4(213, 227, 343, 332), 554, 3));
        scene.AddObject(std::make_shared<XzRect>(glm::vec4(0, 0, 555, 555), 0, 1));
        scene.AddObject(std::make_shared<XzRect>(glm::vec4(0, 0, 555, 555), 555, 1));
        scene.AddObject(std::make_shared<XyRect>(glm::vec4(0, 0, 555, 555), 555, 1));

    }
    return scene;
}

int main(int argc, char** args) {
    // init
    constexpr float aspect      = 1.0f;
    constexpr int   imageHeight = 600, imageWidth = static_cast<int>(imageHeight * aspect);

    auto  renderer = Renderer(imageWidth, imageHeight);
    auto  camera   = std::make_shared<Camera>(cameraOrgin, lookat, updir, aspect, verticalFov);

    // bind scene and camera
    renderer.BindScene(InitScene(1));
    renderer.BindCamera(camera);
    // start render
    renderer.Render();

    return 0;
}