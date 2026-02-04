#include "LUTManager.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cmath>

LUTManager::LUTManager() {
}

bool LUTManager::loadLUT(const std::string& path) {
    std::ifstream file(path);
    
    if (!file.is_open()) {
        std::cerr << "Impossible d'ouvrir le fichier LUT: " << path << std::endl;
        return false;
    }
    
    m_lut = LUT3D();
    m_lut.name = path;
    
    std::string line;
    bool readingData = false;
    
    while (std::getline(file, line)) {
        // Ignorer les lignes vides et commentaires
        if (line.empty() || line[0] == '#') {
            continue;
        }
        
        // Chercher la taille de la LUT
        if (line.find("LUT_3D_SIZE") != std::string::npos) {
            std::istringstream iss(line);
            std::string keyword;
            iss >> keyword >> m_lut.size;
            
            // Allouer l'espace pour les données
            int totalPoints = m_lut.size * m_lut.size * m_lut.size;
            m_lut.data.reserve(totalPoints * 3);
            
            readingData = true;
            continue;
        }
        
        // Lire les données RGB
        if (readingData) {
            std::istringstream iss(line);
            float r, g, b;
            
            if (iss >> r >> g >> b) {
                m_lut.data.push_back(r);
                m_lut.data.push_back(g);
                m_lut.data.push_back(b);
            }
        }
    }
    
    file.close();
    
    // Vérifier que nous avons le bon nombre de données
    int expectedSize = m_lut.size * m_lut.size * m_lut.size * 3;
    if (static_cast<int>(m_lut.data.size()) != expectedSize) {
        std::cerr << "Taille de LUT incorrecte. Attendu: " << expectedSize 
                  << ", Reçu: " << m_lut.data.size() << std::endl;
        m_lut = LUT3D();
        return false;
    }
    
    std::cout << "LUT chargée: " << m_lut.name 
              << " (taille: " << m_lut.size << ")" << std::endl;
    
    return true;
}

void LUTManager::applyLUT(float& r, float& g, float& b) const {
    if (!m_lut.isValid()) {
        return;
    }
    
    trilinearInterpolate(r, g, b, r, g, b);
}

void LUTManager::clearLUT() {
    m_lut = LUT3D();
}

void LUTManager::trilinearInterpolate(float r, float g, float b, 
                                      float& outR, float& outG, float& outB) const {
    // Clamper les entrées
    r = clamp(r, 0.0f, 1.0f);
    g = clamp(g, 0.0f, 1.0f);
    b = clamp(b, 0.0f, 1.0f);
    
    // Convertir [0,1] en indices de grille
    float scale = static_cast<float>(m_lut.size - 1);
    float rScaled = r * scale;
    float gScaled = g * scale;
    float bScaled = b * scale;
    
    // Indices des coins du cube
    int r0 = static_cast<int>(std::floor(rScaled));
    int g0 = static_cast<int>(std::floor(gScaled));
    int b0 = static_cast<int>(std::floor(bScaled));

    
    int r1 = std::min(r0 + 1, m_lut.size - 1);
    int g1 = std::min(g0 + 1, m_lut.size - 1);
    int b1 = std::min(b0 + 1, m_lut.size - 1);
    
    // Facteurs d'interpolation
    float rFrac = rScaled - r0;
    float gFrac = gScaled - g0;
    float bFrac = bScaled - b0;
    
    // Interpolation trilinéaire (8 coins du cube)
    auto getLUTValue = [this](int ri, int gi, int bi, int channel) -> float {
        int index = (ri * m_lut.size * m_lut.size + gi * m_lut.size + bi) * 3 + channel;
        return m_lut.data[index];
    };
    
    for (int ch = 0; ch < 3; ++ch) {
        float c000 = getLUTValue(r0, g0, b0, ch);
        float c001 = getLUTValue(r0, g0, b1, ch);
        float c010 = getLUTValue(r0, g1, b0, ch);
        float c011 = getLUTValue(r0, g1, b1, ch);
        float c100 = getLUTValue(r1, g0, b0, ch);
        float c101 = getLUTValue(r1, g0, b1, ch);
        float c110 = getLUTValue(r1, g1, b0, ch);
        float c111 = getLUTValue(r1, g1, b1, ch);
        
        // Interpolation en X
        float c00 = c000 + rFrac * (c100 - c000);
        float c01 = c001 + rFrac * (c101 - c001);
        float c10 = c010 + rFrac * (c110 - c010);
        float c11 = c011 + rFrac * (c111 - c011);
        
        // Interpolation en Y
        float c0 = c00 + gFrac * (c10 - c00);
        float c1 = c01 + gFrac * (c11 - c01);
        
        // Interpolation en Z
        float result = c0 + bFrac * (c1 - c0);
        
        if (ch == 0) outR = result;
        else if (ch == 1) outG = result;
        else outB = result;
    }
}

float LUTManager::clamp(float value, float min, float max) const {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}