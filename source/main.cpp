#include "renderer.h"

static constexpr glm::vec3 cameraOrigin = {278, 278, -800};
static constexpr glm::vec3 lookat       = {278, 278, 0};
static constexpr glm::vec3 updir        = {0.0f, 1.0f, 0.0f};
static constexpr float     verticalFov  = 40.0f;
static constexpr float     aspect       = 1.0f;
static constexpr int       imageHeight = 600, imageWidth = static_cast<int>(imageHeight * aspect);

Scene CreateScene() {
    Scene scene;
    
    Material red   = {glm::vec3(.65f, .05f, .05f), black, 0, MaterialType::diffuse};
    Material white = {glm::vec3(.73, .73, .73), black, 0, MaterialType::diffuse};
    Material green = {glm::vec3(.12, .45, .15), black, 0, MaterialType::diffuse};
    Material light = {{1.0f, 1.0f, 1.0f}, {15.0f, 15.0f, 15.0f}, 0, MaterialType::light};

    scene.AddMaterial(red);
    scene.AddMaterial(white); 
    scene.AddMaterial(green);
    scene.AddMaterial(light);

    std::shared_ptr<Box> box1 = std::make_shared<Box>(glm::vec3(0, 0, 0), glm::vec3(165, 330, 165), 1);
    std::shared_ptr<RotateY> rotateBox1 = std::make_shared<RotateY>(box1, 15);
    std::shared_ptr<Translate> translateBox1 = std::make_shared<Translate>(rotateBox1, glm::vec3(265, 0, 295));
    scene.AddObject(translateBox1);

    std::shared_ptr<Box> box2 = std::make_shared<Box>(glm::vec3(0, 0, 0), glm::vec3(165, 165, 165), 1);
    std::shared_ptr<RotateY> rotateBox2 = std::make_shared<RotateY>(box2, -18);
    std::shared_ptr<Translate> translateBox2 = std::make_shared<Translate>(rotateBox2, glm::vec3(130, 0, 65));
    scene.AddObject(translateBox2);
    
    scene.AddObject(std::make_shared<YzRect>(glm::vec4(0, 0, 555, 555), 555, 2));
    scene.AddObject(std::make_shared<YzRect>(glm::vec4(0, 0, 555, 555), 0, 0));
    scene.AddObject(std::make_shared<XzRect>(glm::vec4(213, 227, 343, 332), 554, 3));
    scene.AddObject(std::make_shared<XzRect>(glm::vec4(0, 0, 555, 555), 0, 1));
    scene.AddObject(std::make_shared<XzRect>(glm::vec4(0, 0, 555, 555), 555, 1));
    scene.AddObject(std::make_shared<XyRect>(glm::vec4(0, 0, 555, 555), 555, 1));
    
    return scene;
}

int main(int argc, char** args) {
    // init
    auto renderer = Renderer(imageWidth, imageHeight);
    // bind scene and camera
    renderer.BindScene(CreateScene());
    renderer.BindCamera(std::make_shared<Camera>(cameraOrigin, lookat, updir, aspect, verticalFov));
    // start render
    renderer.Render();
    return 0;
}

