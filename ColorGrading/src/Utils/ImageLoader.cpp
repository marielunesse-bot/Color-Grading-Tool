#include "ImageLoader.hpp"

// stb_image pour le chargement
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// stb_image_write pour la sauvegarde
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <iostream>

std::string ImageLoader::s_lastError = "";

RawImage ImageLoader::loadRaw(const std::string& path) {
    RawImage img;
    
    // DEBUG: Afficher le chemin complet
    std::cout << "Tentative de chargement: " << path << std::endl;
    
    // Vérifier si le fichier existe
    FILE* test = fopen(path.c_str(), "rb");
    if (test) {
        std::cout << "Fichier trouvé et accessible!" << std::endl;
        fclose(test);
    } else {
        std::cout << "ERREUR: Impossible d'ouvrir le fichier!" << std::endl;
    }
    
    // Charger avec stb_image
    unsigned char* data = stbi_load(
        path.c_str(), 
        &img.width, 
        &img.height, 
        &img.channels, 
        0 // 0 = garder le nombre de channels original
    );
    
    if (!data) {
        s_lastError = "Impossible de charger l'image: " + std::string(stbi_failure_reason());
        std::cerr << s_lastError << std::endl;
        return img;
    }
    
    // Copier les données dans le vecteur
    size_t dataSize = img.width * img.height * img.channels;
    img.data.assign(data, data + dataSize);
    
    // Libérer la mémoire stb_image
    stbi_image_free(data);
    
    std::cout << "Image chargée: " << img.width << "x" << img.height 
              << " (" << img.channels << " channels)" << std::endl;
    
    s_lastError = "";
    return img;
}

bool ImageLoader::saveRaw(const std::string& path, const RawImage& img) {
    if (!img.isValid()) {
        s_lastError = "Image invalide";
        return false;
    }
    
    // Détecter le format depuis l'extension
    std::string ext = path.substr(path.find_last_of('.') + 1);
    
    int result = 0;
    if (ext == "png" || ext == "PNG") {
        result = stbi_write_png(
            path.c_str(), 
            img.width, 
            img.height, 
            img.channels, 
            img.data.data(), 
            img.width * img.channels
        );
    } else if (ext == "jpg" || ext == "jpeg" || ext == "JPG" || ext == "JPEG") {
        result = stbi_write_jpg(
            path.c_str(), 
            img.width, 
            img.height, 
            img.channels, 
            img.data.data(), 
            90 // Qualité JPEG
        );
    } else if (ext == "bmp" || ext == "BMP") {
        result = stbi_write_bmp(
            path.c_str(), 
            img.width, 
            img.height, 
            img.channels, 
            img.data.data()
        );
    } else {
        s_lastError = "Format non supporté: " + ext;
        return false;
    }
    
    if (result == 0) {
        s_lastError = "Erreur lors de la sauvegarde";
        return false;
    }
    
    std::cout << "Image sauvegardée: " << path << std::endl;
    s_lastError = "";
    return true;
}

FloatImage ImageLoader::toFloat(const RawImage& raw) {
    FloatImage result;
    
    if (!raw.isValid()) {
        return result;
    }
    
    result.width = raw.width;
    result.height = raw.height;
    result.channels = 4; // On convertit toujours en RGBA
    
    size_t pixelCount = raw.width * raw.height;
    result.data.resize(pixelCount * 4);
    
    for (size_t i = 0; i < pixelCount; ++i) {
        size_t srcIdx = i * raw.channels;
        size_t dstIdx = i * 4;
        
        // R, G, B
        result.data[dstIdx + 0] = raw.data[srcIdx + 0] / 255.0f;
        result.data[dstIdx + 1] = raw.channels > 1 ? raw.data[srcIdx + 1] / 255.0f : result.data[dstIdx + 0];
        result.data[dstIdx + 2] = raw.channels > 2 ? raw.data[srcIdx + 2] / 255.0f : result.data[dstIdx + 0];
        
        // Alpha
        result.data[dstIdx + 3] = raw.channels == 4 ? raw.data[srcIdx + 3] / 255.0f : 1.0f;
    }
    
    return result;
}

RawImage ImageLoader::fromFloat(const FloatImage& floatImg) {
    RawImage result;
    
    if (!floatImg.isValid()) {
        return result;
    }
    
    result.width = floatImg.width;
    result.height = floatImg.height;
    result.channels = floatImg.channels;
    
    size_t dataSize = floatImg.width * floatImg.height * floatImg.channels;
    result.data.resize(dataSize);
    
    for (size_t i = 0; i < dataSize; ++i) {
        // Clamper [0, 1] et convertir en [0, 255]
        float value = floatImg.data[i];
        if (value < 0.0f) value = 0.0f;
        if (value > 1.0f) value = 1.0f;
        result.data[i] = static_cast<unsigned char>(value * 255.0f);
    }
    
    return result;
}

std::string ImageLoader::getLastError() {
    return s_lastError;
}