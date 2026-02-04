/*
 * ImageLoader - Wrapper pour stb_image
 * Charge des images (PNG, JPG, BMP) et les convertit en format interne
 */

#pragma once // Il empêche d'inclure le même fichier plusieurs fois lors de la compilation.

#include <string>
#include <vector>
#include <memory>

// Structure pour image brute (unsigned char)
struct RawImage {
    int width = 0;
    int height = 0;
    int channels = 0;
   std::vector<unsigned char> data;
    
    bool isValid() const {
        return width > 0 && height > 0 && !data.empty();
    }
};

// Structure pour image en float (meilleure précision)
struct FloatImage {
    int width = 0;
    int height = 0;
    int channels = 4; // Toujours RGBA
   std::vector<float> data; // Valeurs [0.0, 1.0]
    
    bool isValid() const {
        return width > 0 && height > 0 && !data.empty();
    }
};

class ImageLoader {
public:
    // Charger une image depuis un fichier
    static RawImage loadRaw(const std::string& path);
    
    // Sauvegarder une image
    static bool saveRaw(const std::string& path, const RawImage& img);
    
    // Conversions
    static FloatImage toFloat(const RawImage& raw);
    static RawImage fromFloat(const FloatImage& floatImg);
    
    // Obtenir le dernier message d'erreur
    static std::string getLastError();
    
private:
    static std::string s_lastError;
};