#include "Application.h"
#include "ImageLoader.h"
#include "Logger.h"
#include "Window.h"
#include "Texture.h"
#include "Lut.h"
#include "GuiManager.h"
Lut* mLut = nullptr;
Texture* mTexture = nullptr;
ImageLoader mImageLoader;  //Pour garder l'image
GuiManager* mGuiManager = nullptr;
SDL_GLContext mGlContext = nullptr;
Application::Application()
    : mIsRunning(false)
{
}

Application::~Application()
{
    Shutdown();
}

void Application::Init()
{
    Logger::Info("Application initialization...");

    mWindow = new Window("Color Grading Tool", 1280, 720);

    // Init OpenGL context pour ImGui
    mGlContext = SDL_GL_CreateContext(mWindow->GetSDLWindow());

    // Charger l'image et texture
    mImageLoader.LoadFromFile("assets/images/test.jpg");
    mTexture = new Texture();
    mTexture->LoadFromMat(mImageLoader.GetImage());

    // Charger LUT
    mLut = new Lut();
    mLut->LoadFromCubeFile("assets/luts/neutral.cube");

    // Init GUI
    mGui = new GuiManager(mWindow->GetSDLWindow(), mGlContext);

    mIsRunning = true;
}

void Application::Update()
{
    if (!mWindow) return;

    mWindow->PollEvents(mIsRunning);

    mWindow->Clear();

    // TODO: Bind texture + shader avec LUT + color grading
    if (mTexture) mTexture->Bind();

    // GUI
    mGui->StartFrame();
    mGui->Render();

    mWindow->Present();

    if (mTexture) mTexture->Unbind();
}

void Application::Shutdown()
{
    Logger::Info("Application shutdown.");
    void Application::Shutdown()
{
    if (mGui)
    {
        mGui->Shutdown();
        delete mGui;
    }

    if (mTexture) delete mTexture;
    if (mLut) delete mLut;
    if (mWindow) delete mWindow;

    Logger::Info("Application shutdown.");
}

void Application::Run()
{
    Init();

    while (mIsRunning)
    {
        Logger::Info("Width: " + std::to_string(imageLoader.GetWidth(1920)));
        Logger::Info("Height: " + std::to_string(imageLoader.GetHeight(1080)));
        Logger::Info("Channels: " + std::to_string(imageLoader.GetChannels(3)));
    }

    mIsRunning = true;
}

void Application::Update()
{
    Logger::Info("Application running...");
    mIsRunning = false; // temporaire : une seule itération
}

void Application::Shutdown()
{
    Logger::Info("Application shutdown.");
}

void Application::Run()
{
    Init();

    while (mIsRunning)
    {
        Update();
    }
}