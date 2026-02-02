#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "Math/Vector2.h"
#include "Log.h"

namespace ColorGradingTool {
    constexpr int WINDOWPOS_CENTERED = 0x2FFF0000;

    /**
     * @brief Propriétés de configuration de l'application
     */
    struct ApplicationProperties {
            // Configuration de la fenêtre
            std::string windowTitle = "ColorGrading Application";
            int windowWidth = 800;
            int windowHeight = 600;
            math::Vector2 windowPosition = {WINDOWPOS_CENTERED, WINDOWPOS_CENTERED};
            
            // Options de fenêtre
            bool resizable = false;
            bool fullscreen = false;
            bool borderless = false;
            bool highDPI = false;
            
            // Configuration de l'application
            std::string applicationName = "ColorGradingApp";
            std::string organizationName = "ColorGrading";
            
            // Configuration du rendu
            bool vsync = true;
            int targetFPS = 60;
            
            // Chemins des ressources
            std::string iconPath;
            std::string fontPath;
            
            // Arguments de ligne de commande
            std::vector<std::string> commandLineArgs;
            
            /**
             * @brief Crée des propriétés à partir d'arguments de ligne de commande
             */
            static ApplicationProperties FromCommandLine(const std::vector<std::string>& args);
            
            /**
             * @brief Charge les propriétés depuis un fichier de configuration
             */
            static ApplicationProperties FromConfigFile(const std::string& configPath);
            
            /**
             * @brief Combine les propriétés de la ligne de commande et du fichier de config
             * Les valeurs de la ligne de commande ont priorité sur le fichier de config
             */
            static ApplicationProperties FromCommandLineAndConfig(const std::vector<std::string>& args);
            
        private:
            /**
             * @brief Parse une ligne de configuration
             */
            static void ParseConfigLine(ApplicationProperties& props, const std::string& line);
    };

} // namespace ColorGradingTool