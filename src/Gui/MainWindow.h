#pragma once

#include <SDL3/SDL.h>
#include <string>

class Window
{
public:
    Window(const std::string& title, int width, int height);
    ~Window();

    void PollEvents(bool& isRunning);
    void Clear();
    void Present();

private:
    SDL_Window* mWindow;
    SDL_Renderer* mRenderer;
};