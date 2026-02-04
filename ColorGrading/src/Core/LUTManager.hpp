#pragma once

#include <string>
#include <vector>

// Structure pour LUT 3D
struct LUT3D {
    int size;                    // Taille de la grille (ex: 33 pour 33x33x33)
    std::vector<float> data;     // RGB values [size^3 * 3]
    std::string name;
    
    LUT3D() : size(0) {}
    
    bool isValid() const {
        return size > 0 && !data.empty();
    }
};

class LUTManager {
public:
    LUTManager();
    
    // Chargement
    bool loadLUT(const std::string& path);
    bool hasLUT() const { return m_lut.isValid(); }
    
    // Application
    void applyLUT(float& r, float& g, float& b) const;
    
    // Accesseurs
    const std::string& getCurrentLUTName() const { return m_lut.name; }
    int getLUTSize() const { return m_lut.size; }
    
    // Reset
    void clearLUT();
    
private:
    LUT3D m_lut;
    
    // Trilinear interpolation dans la LUT 3D
    void trilinearInterpolate(float r, float g, float b, 
                              float& outR, float& outG, float& outB) const;
    
    float clamp(float value, float min, float max) const;
};