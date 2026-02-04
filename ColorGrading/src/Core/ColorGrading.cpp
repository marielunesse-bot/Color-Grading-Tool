#include "ColorGrading.hpp"
#include <iostream>
#include <algorithm>
#include <cmath>

ColorGrading::ColorGrading() 
    : m_texture(nullptr)
    , m_textureNeedsUpdate(false)
    , m_brightness(0.0f)
    , m_contrast(0.0f)
    , m_saturation(1.0f)
{
}

ColorGrading::~ColorGrading() {
    if (m_texture) {
        SDL_DestroyTexture(m_texture);
    }
}

bool ColorGrading::loadImage(const std::string& path) {
    // Charger l'image brute
    RawImage raw = ImageLoader::loadRaw(path);
    
    if (!raw.isValid()) {
        std::cerr << "Erreur: " << ImageLoader::getLastError() << std::endl;
        return false;
    }
    
    // Convertir en float
    m_originalImage = ImageLoader::toFloat(raw);
    m_currentImage = m_originalImage;
    
    // Reset paramètres
    resetParameters();
    
    m_textureNeedsUpdate = true;
    
    std::cout << "Image chargee avec succes!" << std::endl;
    return true;
}

bool ColorGrading::exportImage(const std::string& path) {
    if (!m_currentImage.isValid()) {
        std::cerr << "Aucune image a exporter" << std::endl;
        return false;
    }
    
    // Convertir en raw
    RawImage raw = ImageLoader::fromFloat(m_currentImage);
    
    // Sauvegarder
    return ImageLoader::saveRaw(path, raw);
}

void ColorGrading::resetToOriginal() {
    if (m_originalImage.isValid()) {
        m_currentImage = m_originalImage;
        resetParameters();
        m_textureNeedsUpdate = true;
    }
}

void ColorGrading::resetParameters() {
    m_brightness = 0.0f;
    m_contrast = 0.0f;
    m_saturation = 1.0f;
}

void ColorGrading::setBrightness(float value) {
    m_brightness = clamp(value, -1.0f, 1.0f);
    applyTransformations();
}

void ColorGrading::setContrast(float value) {
    m_contrast = clamp(value, -1.0f, 1.0f);
    applyTransformations();
}

void ColorGrading::setSaturation(float value) {
    m_saturation = clamp(value, 0.0f, 2.0f);
    applyTransformations();
}

void ColorGrading::applyTransformations() {
    if (!m_originalImage.isValid()) {
        return;
    }
    
    // Partir de l'image originale
    m_currentImage = m_originalImage;
    
    // Générer les LUTs des courbes pour performance
    auto rgbLUT = m_rgbCurve.generateLUT(256);
    auto redLUT = m_redCurve.generateLUT(256);
    auto greenLUT = m_greenCurve.generateLUT(256);
    auto blueLUT = m_blueCurve.generateLUT(256);
    
    size_t pixelCount = m_currentImage.width * m_currentImage.height;
    
    for (size_t i = 0; i < pixelCount; ++i) {
        size_t idx = i * 4;
        
        float r = m_currentImage.data[idx + 0];
        float g = m_currentImage.data[idx + 1];
        float b = m_currentImage.data[idx + 2];
        
        // 1. Brightness
        r += m_brightness;
        g += m_brightness;
        b += m_brightness;
        
        // 2. Contrast
        float contrastFactor = (m_contrast + 1.0f);
        r = (r - 0.5f) * contrastFactor + 0.5f;
        g = (g - 0.5f) * contrastFactor + 0.5f;
        b = (b - 0.5f) * contrastFactor + 0.5f;
        
        // 3. Saturation
        float h, s, v;
        rgbToHsv(r, g, b, h, s, v);
        s *= m_saturation;
        s = clamp(s, 0.0f, 1.0f);
        hsvToRgb(h, s, v, r, g, b);
        
        // Clamper avant courbes
        r = clamp(r, 0.0f, 1.0f);
        g = clamp(g, 0.0f, 1.0f);
        b = clamp(b, 0.0f, 1.0f);
        
        // 4. Appliquer les courbes via LUT
        int rIdx = static_cast<int>(r * 255.0f);
        int gIdx = static_cast<int>(g * 255.0f);
        int bIdx = static_cast<int>(b * 255.0f);
        
        // Courbe RGB (maître)
        r = rgbLUT[rIdx];
        g = rgbLUT[gIdx];
        b = rgbLUT[bIdx];
        
        // Courbes individuelles
        rIdx = static_cast<int>(r * 255.0f);
        gIdx = static_cast<int>(g * 255.0f);
        bIdx = static_cast<int>(b * 255.0f);
        
        r = redLUT[rIdx];
        g = greenLUT[gIdx];
        b = blueLUT[bIdx];
        
        // 5. Appliquer LUT 3D si chargée
        if (m_lutManager.hasLUT()) {
            m_lutManager.applyLUT(r, g, b);
        }
        
        // Clamper les valeurs finales
        m_currentImage.data[idx + 0] = clamp(r, 0.0f, 1.0f);
        m_currentImage.data[idx + 1] = clamp(g, 0.0f, 1.0f);
        m_currentImage.data[idx + 2] = clamp(b, 0.0f, 1.0f);
    }
    
    m_textureNeedsUpdate = true;
}

SDL_Texture* ColorGrading::getTexture(SDL_Renderer* renderer) {
    if (!m_currentImage.isValid()) {
        return nullptr;
    }
    
    if (m_textureNeedsUpdate) {
        updateTexture(renderer);
    }
    
    return m_texture;
}

void ColorGrading::updateTexture(SDL_Renderer* renderer) {
    // Détruire l'ancienne texture
    if (m_texture) {
        SDL_DestroyTexture(m_texture);
        m_texture = nullptr;
    }
    
    // Créer la nouvelle texture
    m_texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA32,
        SDL_TEXTUREACCESS_STATIC,
        m_currentImage.width,
        m_currentImage.height
    );
    
    if (!m_texture) {
        std::cerr << "Erreur creation texture: " << SDL_GetError() << std::endl;
        return;
    }
    
    // Convertir float -> unsigned char pour SDL
    RawImage raw = ImageLoader::fromFloat(m_currentImage);
    
    // Mettre à jour la texture
    SDL_UpdateTexture(
        m_texture,
        nullptr,
        raw.data.data(),
        m_currentImage.width * 4
    );
    
    m_textureNeedsUpdate = false;
}

float ColorGrading::clamp(float value, float min, float max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

void ColorGrading::rgbToHsv(float r, float g, float b, float& h, float& s, float& v) {
    float max = std::max({r, g, b});
    float min = std::min({r, g, b});
    float delta = max - min;
    
    v = max;
    
    if (max < 0.0001f) {
        s = 0.0f;
        h = 0.0f;
        return;
    }
    
    s = delta / max;
    
    if (delta < 0.0001f) {
        h = 0.0f;
        return;
    }
    
    if (max == r) {
        h = 60.0f * fmod(((g - b) / delta), 6.0f);
    } else if (max == g) {
        h = 60.0f * (((b - r) / delta) + 2.0f);
    } else {
        h = 60.0f * (((r - g) / delta) + 4.0f);
    }
    
    if (h < 0.0f) h += 360.0f;
}

void ColorGrading::hsvToRgb(float h, float s, float v, float& r, float& g, float& b) {
    if (s < 0.0001f) {
        r = g = b = v;
        return;
    }
    
    h = fmod(h, 360.0f);
    h /= 60.0f;
    
    int i = static_cast<int>(h);
    float f = h - i;
    
    float p = v * (1.0f - s);
    float q = v * (1.0f - s * f);
    float t = v * (1.0f - s * (1.0f - f));
    
    switch (i) {
        case 0: r = v; g = t; b = p; break;
        case 1: r = q; g = v; b = p; break;
        case 2: r = p; g = v; b = t; break;
        case 3: r = p; g = q; b = v; break;
        case 4: r = t; g = p; b = v; break;
        default: r = v; g = p; b = q; break;
    }
}

bool ColorGrading::loadLUT(const std::string& path) {
    bool success = m_lutManager.loadLUT(path);
    if (success) {
        applyTransformations();
    }
    return success;
}

void ColorGrading::clearLUT() {
    m_lutManager.clearLUT();
    applyTransformations();
}