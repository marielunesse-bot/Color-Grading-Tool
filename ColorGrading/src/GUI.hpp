#pragma once

#include <functional>
#include <string>

// Forward declaration
class ColorGrading;

class GUI {
public:
    GUI();
    ~GUI();
    
    void render(ColorGrading* colorGrading, bool hasImage);
    
    // Callbacks
    std::function<void(const std::string&)> onLoadImage;
    std::function<void(const std::string&)> onExportImage;
    std::function<void()> onResetImage;
    std::function<void()> onQuit;
    std::function<void(const std::string&)> onLoadLUT;
    
private:
    void renderMenuBar();
    void renderImagePreview(ColorGrading* colorGrading);
    void renderColorControls(ColorGrading* colorGrading);
    void renderCurveEditor(ColorGrading* colorGrading);
    void renderLUTControls(ColorGrading* colorGrading);
    void renderFileDialogs();
    
    // État GUI
    bool m_showControls;
    bool m_showCurves;
    bool m_showLUT;
    float m_previewScale;
    
    // Courbe sélectionnée
    int m_selectedCurve;
    int m_selectedPoint;
    bool m_isDraggingPoint;
};