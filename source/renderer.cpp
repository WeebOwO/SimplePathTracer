#include "renderer.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_sdlrenderer.h"

Renderer::Renderer(int viewPortWidth, int viewPortHeight)
    : m_viewPortWidth(viewPortWidth), m_viewPortHeight(viewPortHeight) {
    Init();
}

Renderer::~Renderer() {
    // // clean up ImGui context
    ImGui_ImplSDLRenderer_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    // // Clean up SDL context
    SDL_DestroyWindow(m_window);
    SDL_DestroyRenderer(m_renderer);
    SDL_Quit();

    delete [] m_frameBuffer;
}

int Renderer::Init() {
    // SDL init part
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0) {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }
    SDL_WindowFlags window_flags =
        (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    m_window = SDL_CreateWindow("Raytracer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                m_viewPortWidth, m_viewPortHeight, window_flags);
    m_renderer =
        SDL_CreateRenderer(m_window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    m_swapBuffer =
        SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
                          m_viewPortWidth, m_viewPortHeight);
    if (m_renderer == nullptr) {
        SDL_Log("Error creating SDL_Renderer!");
        return 0;
    }

    // Imgui init part
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.IniFilename = "imgui.ini";
    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForSDLRenderer(m_window, m_renderer);
    ImGui_ImplSDLRenderer_Init(m_renderer);

    m_frameBuffer = new uint32_t[m_viewPortWidth * m_viewPortHeight];
    return 1;
}

void Renderer::Render() {
    bool done                = false;
    bool show_demo_window    = true;
    bool show_another_window = false;

    while (!done) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT) done = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE &&
                event.window.windowID == SDL_GetWindowID(m_window)) {
                done = true;
            }
            if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                uint32_t width = event.window.data1, height = event.window.data2;
                OnResize(width, height);
                SDL_Log("width is %d, height is %d", width, height);
            }
        }
        RenderUI();
        RenderImage();
        RenderPresent();
    }
}

void Renderer::RenderPresent() {
    ImGui::Render();
    ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
    SDL_UpdateTexture(m_swapBuffer, nullptr, m_frameBuffer, sizeof(uint32_t) * m_viewPortWidth);
    SDL_RenderCopy(m_renderer, m_swapBuffer, nullptr, nullptr);
    SDL_RenderPresent(m_renderer);
}

void Renderer::RenderClear() {
    auto r = static_cast<int>(m_backColor.r * 255.0f);
    auto g = static_cast<int>(m_backColor.g * 255.0f);
    auto b = static_cast<int>(m_backColor.b * 255.0f);
    auto a = static_cast<int>(1.0f * 255.0f);
    SDL_SetRenderDrawColor(m_renderer, r, g, b, a);
    SDL_RenderClear(m_renderer);
}

void Renderer::RenderImage() {
    for (uint32_t y = 0; y < m_viewPortHeight; ++y) {
        for (uint32_t x = 0; x < m_viewPortWidth; ++x) {
            PixelShader(x, y);
        }
    }
}

void Renderer::RenderUI() {
    // UI Scene
    ImGui_ImplSDLRenderer_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
}

void Renderer::OnResize(uint32_t width, uint32_t height) {
    if (width == m_viewPortWidth && height == m_viewPortHeight) { return; }
    m_viewPortWidth = width, m_viewPortHeight = height;
    m_frameBuffer = new uint32_t[width * height];
}

void Renderer::PixelShader(uint32_t x, uint32_t y) {
    glm::vec4 color = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
    DrawPixel(x, y, color);
}

void Renderer::DrawPixel(int x, int y, const glm::vec4& color) {
    auto finalColorVec = glm::clamp(color, 0.0f, 1.0f);
    auto r = static_cast<uint8_t>(finalColorVec.r * 255.0f);
    auto g = static_cast<uint8_t>(finalColorVec.g * 255.0f);
    auto b = static_cast<uint8_t>(finalColorVec.b * 255.0f);
    auto a = static_cast<uint8_t>(finalColorVec.a * 255.0f);
    uint32_t finalColor = (a << 24) + (r << 16) + (g << 8) + b;
    *(m_frameBuffer + y * m_viewPortWidth + x) = finalColor;
}