#include "Window.h"
#include "Logger.h"
#include <iostream>

Window::Window(const std::string& title, int width, int height)
    : mWindow(nullptr), mRenderer(nullptr)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        Logger::Error(std::string("SDL_Init Error: ") + SDL_GetError());
        return;
    }

    mWindow = SDL_CreateWindow(title.c_str(),
                               SDL_WINDOWPOS_CENTERED,
                               SDL_WINDOWPOS_CENTERED,
                               width, height,
                               SDL_WINDOW_OPENGL);

    if (!mWindow)
    {
        Logger::Error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
        return;
    }

    mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
    if (!mRenderer)
    {
        Logger::Error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
        return;
    }

    Logger::Info("SDL Window and Renderer initialized successfully.");
}

Window::~Window()
{
    if (mRenderer) SDL_DestroyRenderer(mRenderer);
    if (mWindow) SDL_DestroyWindow(mWindow);
    SDL_Quit();
    Logger::Info("SDL shutdown completed.");
}

void Window::PollEvents(bool& isRunning)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_EVENT_QUIT)
            isRunning = false;
    }
}

void Window::Clear()
{
    if (mRenderer)
        SDL_SetRenderDrawColor(mRenderer, 30, 30, 30, 255);
        SDL_RenderClear(mRenderer);
}

void Window::Present()
{
    if (mRenderer)
        SDL_RenderPresent(mRenderer);
}