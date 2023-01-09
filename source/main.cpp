#include "renderer.h"
#include <memory>

int main(int argc, char** args) {
    auto renderer = std::make_unique<Renderer>(900, 600);
    renderer->Render();
    return 0;
}