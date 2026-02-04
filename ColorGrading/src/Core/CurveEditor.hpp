#pragma once

#include <vector>
// Point de contrôle sur une courbe
struct ControlPoint {
    float x;  // Position horizontale [0, 1]
    float y;  // Position verticale [0, 1]
    
    ControlPoint(float _x = 0.0f, float _y = 0.0f) : x(_x), y(_y) {}
};

class CurveEditor {
public:
    CurveEditor();
    
    // Gestion des points
    void addPoint(float x, float y);
    void removePoint(int index);
    void movePoint(int index, float x, float y);
   int getPointCount() const { return static_cast<int>(m_points.size()); }
    const ControlPoint& getPoint(int index) const { return m_points[index]; }
    
    // Évaluation de la courbe
    float evaluate(float x) const;
    
    // Génération d'une LUT pour application rapide
    std::vector<float> generateLUT(int resolution = 256) const;
    
    // Reset
    void resetToLinear();
    void resetToIdentity();
    
    // Accesseurs
   const std::vector<ControlPoint>& getPoints() const { return m_points; }
    
private:
    std::vector<ControlPoint> m_points;
    
    // Interpolation linéaire entre deux points
    float linearInterpolate(float x, const ControlPoint& p0, const ControlPoint& p1) const;
    
    // Maintenir l'ordre des points par x croissant
    void sortPoints();
    
    // Trouver l'intervalle contenant x
    int findInterval(float x) const;
    
    float clamp(float value, float min, float max) const;
};