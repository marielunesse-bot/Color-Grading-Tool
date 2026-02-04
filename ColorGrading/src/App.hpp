#pragma once

#include <SDL3/SDL.h>
#include <memory>
#include <string> 

// Forward declarations
class ColorGrading;
class GUI;

class App {
public:
    App();
    ~App();
    
    void run();
    
private:
    void init();
    void processEvents();
    void update();
    void render();
    void cleanup();
    
    // Callbacks pour GUI
    void onLoadImage(const std::string& path);
    void onExportImage(const std::string& path);
    void onResetImage();
    void onLoadLUT(const std::string& path);
    
    // SDL
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    bool m_running;
    
    // Core & GUI
    std::unique_ptr<ColorGrading> m_colorGrading;
    std::unique_ptr<GUI> m_gui;
};