/*******************************************************************************
 *  Vector3D.h
 *
 *  (C) 2004 AG Computergraphik
 *           Universitaet Koblenz-Landau
 ******************************************************************************/


#ifndef CG1_VECTOR3D_H
#define CG1_VECTOR3D_H


//--- Konstanten ---------------------------------------------------------------

const int X = 0;
const int Y = 1;
const int Z = 2;


//--- Klassendefinition --------------------------------------------------------

class Vector3D
{
   public:
      Vector3D();
      Vector3D(float x, float y, float z);

      float x() const;
      float y() const;
      float z() const;

      void setX(float value);
      void setY(float value);
      void setZ(float value);
      void setXYZ(float newX, float newY, float newZ);

      float get(int index) const;
      void  set(int index, float value);

      float length() const;
      void  normalize();
	  Vector3D interpolate(Vector3D end, float t);
      
      Vector3D& operator +=(const Vector3D& rhs);
      Vector3D& operator -=(const Vector3D& rhs);

      Vector3D& operator *=(float rhs);
      Vector3D& operator /=(float rhs);


   protected:
      float mElement[3];
};


//--- Verwandte Funktionen -----------------------------------------------------

const Vector3D operator +(const Vector3D& lhs, const Vector3D& rhs);
const Vector3D operator -(const Vector3D& lhs, const Vector3D& rhs);

const Vector3D operator *(float lhs, const Vector3D& rhs);
const Vector3D operator *(const Vector3D& lhs, float rhs);
const Vector3D operator /(const Vector3D& lhs, float rhs);

float dot(const Vector3D& lhs, const Vector3D& rhs);
Vector3D cross(const Vector3D& lhs, const Vector3D& rhs);


#endif   // !CG1_VECTOR3D_H
