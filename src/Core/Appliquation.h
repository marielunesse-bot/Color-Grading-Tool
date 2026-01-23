#pragma once
#include "Window.h"
Window* mWindow = nullptr;
class Application
{
public:
    Application();
    ~Application();

    void Run();

private:
    void Init();
    void Update();
    void Shutdown();

    bool mIsRunning;
};