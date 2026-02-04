#pragma once

#include "Utils/ImageLoader.hpp"
#include <string>
#include <SDL3/SDL.h>
#include <vector>
#include "CurveEditor.hpp"
#include "LUTManager.hpp"
class ColorGrading {
public:
    ColorGrading();
    ~ColorGrading();
    // Courbes de couleur
    CurveEditor* getRGBCurve() { return &m_rgbCurve; }
    CurveEditor* getRedCurve() { return &m_redCurve; }
    CurveEditor* getGreenCurve() { return &m_greenCurve; }
    CurveEditor* getBlueCurve() { return &m_blueCurve; }
     
    // LUT
    LUTManager* getLUTManager() { return &m_lutManager; }
    bool loadLUT(const std::string& path);
    void clearLUT();

    // Chargement/Export
    bool loadImage(const std::string& path);
    bool hasImage() const { return m_currentImage.isValid(); }
    bool exportImage(const std::string& path);
    
    // Transformations de base
    void setBrightness(float value); // [-1.0, 1.0]
    void setContrast(float value);   // [-1.0, 1.0]
    void setSaturation(float value); // [0.0, 2.0]
    
    // Getters pour l'interface
    float getBrightness() const { return m_brightness; }
    float getContrast() const { return m_contrast; }
    float getSaturation() const { return m_saturation; }
    
    // Accesseurs
    const FloatImage& getCurrentImage() const { return m_currentImage; }
    const FloatImage& getOriginalImage() const { return m_originalImage; }
    
    // Reset
    void resetToOriginal();
    void resetParameters();
    
    // Obtenir une texture SDL pour l'affichage
    SDL_Texture* getTexture(SDL_Renderer* renderer);
    
private:
    FloatImage m_originalImage;  // Image source (immutable)
    FloatImage m_currentImage;   // Image avec transformations
    
    // Courbes
    CurveEditor m_rgbCurve;    // Courbe maître RGB
    CurveEditor m_redCurve;    // Courbe canal rouge
    CurveEditor m_greenCurve;  // Courbe canal vert
    CurveEditor m_blueCurve;   // Courbe canal bleu

   // LUT
    LUTManager m_lutManager;
    
    // Paramètres de transformation
    float m_brightness;
    float m_contrast;
    float m_saturation;
    
    SDL_Texture* m_texture;
    bool m_textureNeedsUpdate;
    
    void updateTexture(SDL_Renderer* renderer);
    void applyTransformations();
    
    // Helpers
    float clamp(float value, float min, float max);
    void rgbToHsv(float r, float g, float b, float& h, float& s, float& v);
    void hsvToRgb(float h, float s, float v, float& r, float& g, float& b);
};