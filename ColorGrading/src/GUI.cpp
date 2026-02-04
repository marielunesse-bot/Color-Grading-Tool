#include "GUI.hpp"
#include "Core/ColorGrading.hpp"
#include "Core/CurveEditor.hpp"
#include <imgui.h>
#include <tinyfiledialogs.h>
#include <SDL3/SDL.h>
#include <algorithm>
#include <cmath>

GUI::GUI() 
    : m_showControls(true)
    , m_showCurves(true)
    , m_showLUT(true)
    , m_previewScale(1.0f)
    , m_selectedCurve(0)
    , m_selectedPoint(-1)
    , m_isDraggingPoint(false)
{
}

GUI::~GUI() {
}

void GUI::render(ColorGrading* colorGrading, bool hasImage) {
    ImGui::Begin("Color Grading Tool", nullptr, ImGuiWindowFlags_MenuBar);
    
    renderMenuBar();
    
    if (hasImage) {
        ImVec2 windowSize = ImGui::GetContentRegionAvail();
        float leftWidth = windowSize.x * 0.65f;
        
        ImGui::BeginChild("ImagePreview", ImVec2(leftWidth, 0), true);
        renderImagePreview(colorGrading);
        ImGui::EndChild();
        
        ImGui::SameLine();
        ImGui::BeginChild("Controls", ImVec2(0, 0), true);
        
        if (ImGui::BeginTabBar("ControlTabs")) {
            if (ImGui::BeginTabItem("Basic")) {
                renderColorControls(colorGrading);
                ImGui::EndTabItem();
            }
            
            if (ImGui::BeginTabItem("Curves")) {
                renderCurveEditor(colorGrading);
                ImGui::EndTabItem();
            }
            
            if (ImGui::BeginTabItem("LUT")) {
                renderLUTControls(colorGrading);
                ImGui::EndTabItem();
            }
            
            ImGui::EndTabBar();
        }
        
        ImGui::EndChild();
        
    } else {
        ImGui::Text("No image loaded");
        ImGui::Spacing();
        ImGui::Text("File > Open Image to load an image");
    }
    
    ImGui::End();
    
    renderFileDialogs();
}

void GUI::renderMenuBar() {
    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Open Image...", "Ctrl+O")) {
                const char* filters[] = {"*.png", "*.jpg", "*.jpeg", "*.bmp"};
                const char* path = tinyfd_openFileDialog(
                    "Choose Image File",
                    "",
                    4,
                    filters,
                    "Image Files",
                    0
                );
                if (path && onLoadImage) {
                    onLoadImage(std::string(path));
                }
            }
            
            if (ImGui::MenuItem("Load LUT...", "Ctrl+L")) {
                const char* filters[] = {"*.cube"};
                const char* path = tinyfd_openFileDialog(
                    "Choose LUT File",
                    "",
                    1,
                    filters,
                    "LUT Files",
                    0
                );
                if (path && onLoadLUT) {
                    onLoadLUT(std::string(path));
                }
            }
            
            if (ImGui::MenuItem("Export Image...", "Ctrl+S")) {
                const char* filters[] = {"*.png", "*.jpg", "*.bmp"};
                const char* path = tinyfd_saveFileDialog(
                    "Save Image As",
                    "output.png",
                    3,
                    filters,
                    "Image Files"
                );
                if (path && onExportImage) {
                    onExportImage(std::string(path));
                }
            }
            
            ImGui::Separator();
            
            if (ImGui::MenuItem("Reset Image")) {
                if (onResetImage) {
                    onResetImage();
                }
            }
            
            ImGui::Separator();
            
            if (ImGui::MenuItem("Quit", "Ctrl+Q")) {
                if (onQuit) {
                    onQuit();
                }
            }
            
            ImGui::EndMenu();
        }
        
        if (ImGui::BeginMenu("View")) {
            ImGui::MenuItem("Basic Controls", nullptr, &m_showControls);
            ImGui::MenuItem("Curves", nullptr, &m_showCurves);
            ImGui::MenuItem("LUT", nullptr, &m_showLUT);
            ImGui::EndMenu();
        }
        
        ImGui::EndMenuBar();
    }
}

void GUI::renderImagePreview(ColorGrading* colorGrading) {
    ImGui::Text("Image Preview");
    ImGui::Separator();
    
    SDL_Texture* texture = colorGrading->getTexture(nullptr);
    if (texture) {
        float w, h;
        SDL_GetTextureSize(texture, &w, &h);
        
        ImGui::Text("Size: %.0fx%.0f", w, h);
        
        ImVec2 availSize = ImGui::GetContentRegionAvail();
        float scale = std::min(availSize.x / w, availSize.y / h);
        ImVec2 imageSize(w * scale, h * scale);
        
        float offsetX = (availSize.x - imageSize.x) * 0.5f;
        if (offsetX > 0) {
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offsetX);
        }
        
        ImGui::Image(texture, imageSize);
    }
}

void GUI::renderColorControls(ColorGrading* colorGrading) {
    if (!m_showControls) return;
    
    ImGui::Text("Color Adjustments");
    ImGui::Separator();
    ImGui::Spacing();
    
    float brightness = colorGrading->getBrightness();
    if (ImGui::SliderFloat("Brightness", &brightness, -1.0f, 1.0f, "%.2f")) {
        colorGrading->setBrightness(brightness);
    }
    
    ImGui::Spacing();
    
    float contrast = colorGrading->getContrast();
    if (ImGui::SliderFloat("Contrast", &contrast, -1.0f, 1.0f, "%.2f")) {
        colorGrading->setContrast(contrast);
    }
    
    ImGui::Spacing();
    
    float saturation = colorGrading->getSaturation();
    if (ImGui::SliderFloat("Saturation", &saturation, 0.0f, 2.0f, "%.2f")) {
        colorGrading->setSaturation(saturation);
    }
    
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    
    if (ImGui::Button("Reset All", ImVec2(-1, 0))) {
        colorGrading->resetParameters();
        colorGrading->setBrightness(0.0f);
    }
}

void GUI::renderCurveEditor(ColorGrading* colorGrading) {
    if (!m_showCurves) return;
    
    ImGui::Text("Curve Editor");
    ImGui::Separator();
    ImGui::Spacing();
    
    const char* curveNames[] = { "RGB (Master)", "Red", "Green", "Blue" };
    ImGui::Combo("Channel", &m_selectedCurve, curveNames, 4);
    
    ImGui::Spacing();
    
    CurveEditor* curve = nullptr;
    switch (m_selectedCurve) {
        case 0: curve = colorGrading->getRGBCurve(); break;
        case 1: curve = colorGrading->getRedCurve(); break;
        case 2: curve = colorGrading->getGreenCurve(); break;
        case 3: curve = colorGrading->getBlueCurve(); break;
    }
    
    if (curve) {
        ImVec2 canvasPos = ImGui::GetCursorScreenPos();
        ImVec2 canvasSize = ImGui::GetContentRegionAvail();
        canvasSize.y = std::min(canvasSize.y, 250.0f);
        
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        
        drawList->AddRectFilled(canvasPos, 
                               ImVec2(canvasPos.x + canvasSize.x, canvasPos.y + canvasSize.y),
                               IM_COL32(40, 40, 40, 255));
        
        for (int i = 0; i <= 4; ++i) {
            float x = canvasPos.x + (canvasSize.x / 4) * i;
            float y = canvasPos.y + (canvasSize.y / 4) * i;
            drawList->AddLine(ImVec2(x, canvasPos.y), 
                            ImVec2(x, canvasPos.y + canvasSize.y),
                            IM_COL32(60, 60, 60, 255));
            drawList->AddLine(ImVec2(canvasPos.x, y), 
                            ImVec2(canvasPos.x + canvasSize.x, y),
                            IM_COL32(60, 60, 60, 255));
        }
        
        drawList->AddLine(canvasPos, 
                         ImVec2(canvasPos.x + canvasSize.x, canvasPos.y + canvasSize.y),
                         IM_COL32(80, 80, 80, 255), 1.0f);
        
        auto lut = curve->generateLUT(256);
        for (int i = 0; i < 255; ++i) {
            float x0 = canvasPos.x + (canvasSize.x * i / 255.0f);
            float y0 = canvasPos.y + canvasSize.y * (1.0f - lut[i]);
            float x1 = canvasPos.x + (canvasSize.x * (i + 1) / 255.0f);
            float y1 = canvasPos.y + canvasSize.y * (1.0f - lut[i + 1]);
            
            ImU32 color = IM_COL32(200, 200, 200, 255);
            if (m_selectedCurve == 1) color = IM_COL32(255, 100, 100, 255);
            else if (m_selectedCurve == 2) color = IM_COL32(100, 255, 100, 255);
            else if (m_selectedCurve == 3) color = IM_COL32(100, 100, 255, 255);
            
            drawList->AddLine(ImVec2(x0, y0), ImVec2(x1, y1), color, 2.0f);
        }
        
        const auto& points = curve->getPoints();
        for (int i = 0; i < curve->getPointCount(); ++i) {
            const auto& pt = curve->getPoint(i);
            float px = canvasPos.x + pt.x * canvasSize.x;
            float py = canvasPos.y + (1.0f - pt.y) * canvasSize.y;
            
            ImU32 pointColor = (i == m_selectedPoint) ? IM_COL32(255, 255, 0, 255) : IM_COL32(255, 255, 255, 255);
            drawList->AddCircleFilled(ImVec2(px, py), 5.0f, pointColor);
            drawList->AddCircle(ImVec2(px, py), 5.0f, IM_COL32(0, 0, 0, 255), 12, 2.0f);
        }
        
        ImGui::InvisibleButton("canvas", canvasSize);
        ImVec2 mousePos = ImGui::GetMousePos();
        
        if (ImGui::IsItemHovered()) {
            if (ImGui::IsMouseDoubleClicked(0)) {
                float x = (mousePos.x - canvasPos.x) / canvasSize.x;
                float y = 1.0f - (mousePos.y - canvasPos.y) / canvasSize.y;
                curve->addPoint(x, y);
                colorGrading->setBrightness(colorGrading->getBrightness());
            }
            
            if (ImGui::IsMouseClicked(0)) {
                m_selectedPoint = -1;
                for (int i = 0; i < curve->getPointCount(); ++i) {
                    const auto& pt = curve->getPoint(i);
                    float px = canvasPos.x + pt.x * canvasSize.x;
                    float py = canvasPos.y + (1.0f - pt.y) * canvasSize.y;
                    float dist = std::sqrt((mousePos.x - px) * (mousePos.x - px) + 
                                          (mousePos.y - py) * (mousePos.y - py));
                    if (dist < 8.0f) {
                        m_selectedPoint = i;
                        m_isDraggingPoint = true;
                        break;
                    }
                }
            }
        }
        
        if (m_isDraggingPoint && ImGui::IsMouseDown(0) && m_selectedPoint >= 0) {
            float x = (mousePos.x - canvasPos.x) / canvasSize.x;
            float y = 1.0f - (mousePos.y - canvasPos.y) / canvasSize.y;
            curve->movePoint(m_selectedPoint, x, y);
            colorGrading->setBrightness(colorGrading->getBrightness());
        }
        
        if (ImGui::IsMouseReleased(0)) {
            m_isDraggingPoint = false;
        }
        
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();
        
        if (ImGui::Button("Reset Curve", ImVec2(-1, 0))) {
            curve->resetToLinear();
            colorGrading->setBrightness(colorGrading->getBrightness());
        }
        
        if (m_selectedPoint >= 0 && ImGui::Button("Remove Point", ImVec2(-1, 0))) {
            curve->removePoint(m_selectedPoint);
            m_selectedPoint = -1;
            colorGrading->setBrightness(colorGrading->getBrightness());
        }
    }
}

void GUI::renderLUTControls(ColorGrading* colorGrading) {
    if (!m_showLUT) return;
    
    ImGui::Text("LUT (Look-Up Table)");
    ImGui::Separator();
    ImGui::Spacing();
    
    LUTManager* lutMgr = colorGrading->getLUTManager();
    
    if (lutMgr->hasLUT()) {
        ImGui::Text("Current LUT:");
        ImGui::TextWrapped("%s", lutMgr->getCurrentLUTName().c_str());
        ImGui::Text("Size: %dx%dx%d", lutMgr->getLUTSize(), lutMgr->getLUTSize(), lutMgr->getLUTSize());
        
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();
        
        if (ImGui::Button("Clear LUT", ImVec2(-1, 0))) {
            colorGrading->clearLUT();
        }
    } else {
        ImGui::Text("No LUT loaded");
        ImGui::Spacing();
        ImGui::TextWrapped("Use File > Load LUT to load a .cube file");
    }
    
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    
    ImGui::TextWrapped("LUT files must be in .cube format (Adobe standard)");
}

void GUI::renderFileDialogs() {
    // Fonction vide maintenant car les dialogues sont gérés directement dans renderMenuBar
}