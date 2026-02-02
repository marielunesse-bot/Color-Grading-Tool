#include "Core/Application.h"
#include <SDL3/SDL_main.h>
#include "Core/Log.h"

// Déclaration de la fonction utilisateur
ColorGradingTool* Main(nkentseu::Args args);

int main(int argc, char* argv[]) {
    // Initialiser le système de logging
    ColorGradingTool::Logger::Initialize();
    
    // Convertir les arguments
    ColorGradingTool::Args args;
    for (int i = 0; i < argc; ++i) {
        args.push_back(argv[i]);
    }
    
    ColorGradingTool::Log.Info("Starting RihenNatural application");
    ColorGradingTool::Log.Info("Arguments count: {0}", args.size());
    
    // Appeler la fonction principale de l'utilisateur
    ColorGradingTool::Application* app = Main(args);
    
    if (!app) {
        ColorGradingTool::Log.Error("Main() function returned null pointer");
        ColorGradingTool::Log.Shutdown();
        return 1;
    }
    
    // Initialiser l'application
    if (!app->Initialize()) {
        ColorGradingTool::Log.Error("Failed to initialize application");
        delete app;
        ColorGradingTool::Log.Shutdown();
        return 1;
    }
    
    // Exécuter la boucle principale
    app->Run();
    
    // Nettoyer
    delete app;
    ColorGradingTool::Logger::Shutdown();
    
    return 0;
}