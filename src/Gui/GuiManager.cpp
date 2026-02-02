#include "GuiManager.h"
#include "../Core/Log.h"

GuiManager::GuiManager(SDL_Window* window, SDL_GLContext glContext)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui_ImplSDL3_InitForOpenGL(window, glContext);
    ImGui_ImplOpenGL3_Init("#version 330");

    Logger::Info("ImGui initialized.");
}

GuiManager::~GuiManager()
{
    Shutdown();
}

void GuiManager::StartFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
}

void GuiManager::Render()
{
    ImGui::Begin("Color Grading Tool");

    ImGui::SliderFloat("Exposure", &exposure, 0.0f, 3.0f);
    ImGui::SliderFloat("Contrast", &contrast, 0.0f, 3.0f);
    ImGui::SliderFloat("Saturation", &saturation, 0.0f, 3.0f);

    ImGui::Text("Curves (R/G/B) preview not implemented yet");
    // Ici tu pourras ajouter un plot pour visualiser les courbes

    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GuiManager::Shutdown()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    Logger::Info("ImGui shutdown completed.");
}