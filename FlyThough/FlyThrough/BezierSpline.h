#ifndef _BEZIERSPLINE_H
#define _BEZIERSPLINE_H

#include "Vector3D.h"
#include <glm/glm.hpp>

#include <vector>

using namespace std;

class BezierSpline 
{
public:
    BezierSpline();
    ~BezierSpline();
    
    glm::vec3 deCasteljau(float t);        // Punkt fuer Parameter t bestimmen
    
    void addPoint(Vector3D newPoint);     // neuen Punkt anfuegen
    
    //void drawControlPolygon();            // Kontrollpolygon zeichnen
    void draw(int resolution);            // Spline in <resolution> Schritten zeichnen
    
private:
    vector<Vector3D> _controlPoints;      // Kontrollpunkte  
};

#endif

