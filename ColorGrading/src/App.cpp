#include "App.hpp"
#include "GUI.hpp"
#include "Core/ColorGrading.hpp"
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>
#include <iostream>

App::App() 
    : m_window(nullptr)
    , m_renderer(nullptr)
    , m_running(false)
{
}

App::~App() {
    cleanup();
}

void App::init() {
    // Initialiser SDL3
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        throw std::runtime_error("SDL_Init failed: " + std::string(SDL_GetError()));
    }
    
    // Créer fenêtre
    m_window = SDL_CreateWindow(
        "Color Grading Tool",
        1280, 720,
        SDL_WINDOW_RESIZABLE
    );
    
    if (!m_window) {
        throw std::runtime_error("Window creation failed: " + std::string(SDL_GetError()));
    }
    
    // Créer renderer
    m_renderer = SDL_CreateRenderer(m_window, nullptr);
    if (!m_renderer) {
        throw std::runtime_error("Renderer creation failed: " + std::string(SDL_GetError()));
    }
    
    // Initialiser ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    
    ImGui_ImplSDL3_InitForSDLRenderer(m_window, m_renderer);
    ImGui_ImplSDLRenderer3_Init(m_renderer);
    
    // Style sombre
    ImGui::StyleColorsDark();
    
    // Créer les composants
   m_colorGrading = std::make_unique<ColorGrading>();
   m_gui = std::make_unique<GUI>();
    
    // Configurer les callbacks
    m_gui->onLoadImage = [this](const std::string& path) {
        onLoadImage(path);
    };
    
    m_gui->onExportImage = [this](const std::string& path) {
        onExportImage(path);
    };
    
    m_gui->onResetImage = [this]() {
        onResetImage();
    };
    
    m_gui->onQuit = [this]() {
        m_running = false;
    };
    m_gui->onLoadLUT = [this](const std::string& path) {
    onLoadLUT(path);
    };
    
    m_running = true;
}

void App::run() {
    init();
    
    while (m_running) {
        processEvents();
        update();
        render();
    }
}

void App::processEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        ImGui_ImplSDL3_ProcessEvent(&event);
        
        if (event.type == SDL_EVENT_QUIT) {
            m_running = false;
        }
    }
}

void App::update() {
    // Logique de mise à jour
}

void App::render() {
    // Nouvelle frame ImGui
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
    
    // Rendu de l'interface
    m_gui->render(m_colorGrading.get(), m_colorGrading->hasImage());
    
    // Note: On doit passer le renderer à ColorGrading
    // Pour l'instant on le fait ici de manière temporaire
    if (m_colorGrading->hasImage()) {
        m_colorGrading->getTexture(m_renderer);
    }
    
    // Rendu final
    ImGui::Render();
    SDL_SetRenderDrawColor(m_renderer, 45, 45, 48, 255);
    SDL_RenderClear(m_renderer);
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), m_renderer);
    SDL_RenderPresent(m_renderer);
}

void App::cleanup() {
    m_gui.reset();
    m_colorGrading.reset();
    
    if (m_renderer) {
        ImGui_ImplSDLRenderer3_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext();
        
        SDL_DestroyRenderer(m_renderer);
        m_renderer = nullptr;
    }
    
    if (m_window) {
        SDL_DestroyWindow(m_window);
        m_window = nullptr;
    }
    
    SDL_Quit();
}

void App::onLoadImage(const std::string& path) {
    if (m_colorGrading->loadImage(path)) {
        std::cout << "Image chargee: " << path << std::endl;
    } else {
        std::cerr << "Erreur chargement: " << path << std::endl;
    }
}

void App::onExportImage(const std::string& path) {
    if (m_colorGrading->exportImage(path)) {
        std::cout << "Image exportee: " << path << std::endl;
    }
}

void App::onResetImage() {
    m_colorGrading->resetToOriginal();
    std::cout << "Image reset" << std::endl;
}  // ← FERMETURE ICI

void App::onLoadLUT(const std::string& path) {
    if (m_colorGrading->loadLUT(path)) {
        std::cout << "LUT chargee: " << path << std::endl;
    } else {
        std::cerr << "Erreur chargement LUT: " << path << std::endl;
    }
}