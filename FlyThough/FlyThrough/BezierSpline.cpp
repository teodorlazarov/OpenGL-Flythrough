#include "BezierSpline.h"

#include <iostream>

BezierSpline::BezierSpline()
{
}

BezierSpline::~BezierSpline()
{
}

// *** 3D Punkt fuer Parameter t bestimmen
glm::vec3 BezierSpline::deCasteljau(float t)
{

	vector<Vector3D> points(_controlPoints);
	int length = _controlPoints.size();

	for (int n = length-2; n >=0 ; n--)
	{
		for (int i = 0; i <=n; i++)
		{
			points[i] = points[i].interpolate(points[i + 1], t);	
		}
	}
	return glm::vec3(points[0].x(), points[0].y(), points[0].z());
}

// neuen Punkt anfuegen
void BezierSpline::addPoint(Vector3D newPoint)
{
    _controlPoints.push_back(newPoint);
}

// Kontrollpolygon zeichnen
/*void BezierSpline::drawControlPolygon()
{
  size_t i;
  glDisable(GL_LIGHTING);

    // Kontrollpolygon als Linienzug zeichnen
	glColor3f(1.0f, 1.0f, 0.0f);
	glBegin(GL_LINE_STRIP);
	for (i = 0 ; i < _controlPoints.size() ; i++) {
		glVertex3f(_controlPoints[i].x(), _controlPoints[i].y(), _controlPoints[i].z());
	}
	glEnd();
	
	// Kontrollpunkte zeichnen
	glColor3f(1.0f, 0.0f, 0.0f);
	glPointSize(5.0f);
	glBegin(GL_POINTS);
	for (i = 0 ; i < _controlPoints.size() ; i++) {
		glVertex3f(_controlPoints[i].x(), _controlPoints[i].y(), _controlPoints[i].z());
	}
	glEnd();
	glPointSize(1.0f);
	
    glEnable(GL_LIGHTING);   
}*/

// *** Spline in <resolution> Schritten al GL_LINE_STRIP zeichnen
void BezierSpline::draw(int resolution)
{
	//glDisable(GL_LIGHTING);
	//glBegin(GL_LINE_STRIP);
	for (int i = 0; i <= resolution; i++) {
		glm::vec3 temp = deCasteljau((float)i/resolution);
		std::cout<<"X: " << temp.x << " Y: " << temp.y << " Z: " << temp.z << std::endl;
		//glVertex3f(temp.x(), temp.y(), temp.z());
	}
	//glEnd();
	//glEnable(GL_LIGHTING);
}
 
