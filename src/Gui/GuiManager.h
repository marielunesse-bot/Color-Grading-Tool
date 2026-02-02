#pragma once
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_opengl3.h"

class GuiManager
{
public:
    GuiManager(SDL_Window* window, SDL_GLContext glContext);
    ~GuiManager();

    void StartFrame();
    void Render();
    void Shutdown();

    // Couleurs / sliders
    float exposure = 1.0f;
    float contrast = 1.0f;
    float saturation = 1.0f;

    float curveR[256]{};
    float curveG[256]{};
    float curveB[256]{};
};