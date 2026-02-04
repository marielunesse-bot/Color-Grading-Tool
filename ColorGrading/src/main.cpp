#include "App.hpp"
#include <iostream>
int main(int argc, char* argv[]) {
    try {
        App app;
        app.run();
    } catch (const std::exception& e) {
        std::cerr << "Erreur fatale: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}