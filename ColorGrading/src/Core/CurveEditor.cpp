#include "CurveEditor.hpp"
#include <algorithm>
#include <cmath>

CurveEditor::CurveEditor() {
    resetToLinear();
}

void CurveEditor::addPoint(float x, float y) {
    x = clamp(x, 0.0f, 1.0f);
    y = clamp(y, 0.0f, 1.0f);
    
    m_points.push_back(ControlPoint(x, y));
    sortPoints();
}

void CurveEditor::removePoint(int index) {
    if (index >= 0 && index < static_cast<int>(m_points.size())) {
        // Ne pas supprimer les points d'extrémité (0,0) et (1,1)
        if (m_points[index].x > 0.01f && m_points[index].x < 0.99f) {
            m_points.erase(m_points.begin() + index);
        }
    }
}

void CurveEditor::movePoint(int index, float x, float y) {
    if (index >= 0 && index < static_cast<int>(m_points.size())) {
        x = clamp(x, 0.0f, 1.0f);
        y = clamp(y, 0.0f, 1.0f);
        
        // Les points d'extrémité ne peuvent bouger que verticalement
        if (index == 0) {
            m_points[index].y = y;
            m_points[index].x = 0.0f;
        } else if (index == static_cast<int>(m_points.size()) - 1) {
            m_points[index].y = y;
            m_points[index].x = 1.0f;
        } else {
            m_points[index].x = x;
            m_points[index].y = y;
            sortPoints();
        }
    }
}

float CurveEditor::evaluate(float x) const {
    x = clamp(x, 0.0f, 1.0f);
    
    if (m_points.empty()) {
        return x; // Identité
    }
    
    if (m_points.size() == 1) {
        return m_points[0].y;
    }
    
    // Trouver l'intervalle
    int i = findInterval(x);
    
    if (i < 0) {
        return m_points[0].y;
    }
    
    if (i >= static_cast<int>(m_points.size()) - 1) {
        return m_points.back().y;
    }
    
    // Interpolation linéaire entre les deux points
    return linearInterpolate(x, m_points[i], m_points[i + 1]);
}

std::vector<float> CurveEditor::generateLUT(int resolution) const {
    std::vector<float> lut(resolution);
    
    for (int i = 0; i < resolution; ++i) {
        float x = static_cast<float>(i) / static_cast<float>(resolution - 1);
        lut[i] = evaluate(x);
    }
    
    return lut;
}

void CurveEditor::resetToLinear() {
    m_points.clear();
    m_points.push_back(ControlPoint(0.0f, 0.0f));
    m_points.push_back(ControlPoint(1.0f, 1.0f));
}

void CurveEditor::resetToIdentity() {
    resetToLinear();
}

float CurveEditor::linearInterpolate(float x, const ControlPoint& p0, const ControlPoint& p1) const {
    if (std::abs(p1.x - p0.x) < 0.0001f) {
        return p0.y;
    }
    
    float t = (x - p0.x) / (p1.x - p0.x);
    return p0.y + t * (p1.y - p0.y);
}

void CurveEditor::sortPoints() {
    std::sort(m_points.begin(), m_points.end(), 
              [](const ControlPoint& a, const ControlPoint& b) {
                  return a.x < b.x;
              });
}

int CurveEditor::findInterval(float x) const {
    for (int i = 0; i < static_cast<int>(m_points.size()) - 1; ++i) {
        if (x >= m_points[i].x && x <= m_points[i + 1].x) {
            return i;
        }
    }
    return static_cast<int>(m_points.size()) - 2;
}

float CurveEditor::clamp(float value, float min, float max) const {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}