/*******************************************************************************
 *  Vector3D.cpp
 *
 *  (C) 2004 AG Computergraphik
 *           Universitaet Koblenz-Landau
 ******************************************************************************/

//--- Includes -----------------------------------------------------------------

#include <cassert>
#include <cmath>
#include "Vector3D.h"


//--- Klassenimplementierung ---------------------------------------------------

Vector3D::Vector3D()
{
   setXYZ(0, 0, 0);
}


Vector3D::Vector3D(float x, float y, float z)
{
   setXYZ(x, y, z);
}


float Vector3D::x() const
{
   return mElement[X];
}


float Vector3D::y() const
{
   return mElement[Y];
}


float Vector3D::z() const
{
   return mElement[Z];
}


float Vector3D::get(int index) const
{
   assert(index >= X && index <= Z);
   return mElement[index];
}


void Vector3D::set(int index, float value)
{
   assert(index >= X && index <= Z);
   mElement[index] = value;
}


void Vector3D::setX(float value)
{
   mElement[X] = value;
}


void Vector3D::setY(float value)
{
   mElement[Y] = value;
}


void Vector3D::setZ(float value)
{
   mElement[Z] = value;
}


void Vector3D::setXYZ(float newX, float newY, float newZ)
{
   mElement[X] = newX;
   mElement[Y] = newY;
   mElement[Z] = newZ;
}


float Vector3D::length() const
{
   return sqrt(mElement[X] * mElement[X] +
               mElement[Y] * mElement[Y] +
               mElement[Z] * mElement[Z]);
}


void Vector3D::normalize()
{
   float scale = 1 / length();

   mElement[X] *= scale;
   mElement[Y] *= scale;
   mElement[Z] *= scale;
}

Vector3D Vector3D::interpolate(Vector3D end, float t)
{
	return (1 - t) * (*this) + t * end;
}

Vector3D& Vector3D::operator +=(const Vector3D& rhs)
{
   mElement[X] += rhs.mElement[X];
   mElement[Y] += rhs.mElement[Y];
   mElement[Z] += rhs.mElement[Z];

   return *this;
}


Vector3D& Vector3D::operator -=(const Vector3D& rhs)
{
   mElement[X] -= rhs.mElement[X];
   mElement[Y] -= rhs.mElement[Y];
   mElement[Z] -= rhs.mElement[Z];

   return *this;
}


Vector3D& Vector3D::operator *=(float rhs)
{
   mElement[X] *= rhs;
   mElement[Y] *= rhs;
   mElement[Z] *= rhs;

   return *this;
}


Vector3D& Vector3D::operator /=(float rhs)
{
   mElement[X] /= rhs;
   mElement[Y] /= rhs;
   mElement[Z] /= rhs;

   return *this;
}


//--- Verwandte Funktionen -----------------------------------------------------

const Vector3D operator +(const Vector3D& lhs, const Vector3D& rhs)
{
   return Vector3D(lhs) += rhs;
}


const Vector3D operator -(const Vector3D& lhs, const Vector3D& rhs)
{
   return Vector3D(lhs) -= rhs;
}


const Vector3D operator *(float lhs, const Vector3D& rhs)
{
   return Vector3D(rhs) *= lhs;
}


const Vector3D operator *(const Vector3D& lhs, float rhs)
{
   return Vector3D(lhs) *= rhs;
}


const Vector3D operator /(const Vector3D& lhs, float rhs)
{
   return Vector3D(lhs) /= rhs;
}


float dot(const Vector3D& lhs, const Vector3D& rhs)
{
   return lhs.x() * rhs.x() + lhs.y() * rhs.y() + lhs.z() * rhs.z();
}

Vector3D cross(const Vector3D& lhs, const Vector3D& rhs)
{
   float x = lhs.y() * rhs.z() - lhs.z() * rhs.y();
   float y = lhs.z() * rhs.x() - lhs.x() * rhs.z();
   float z = lhs.x() * rhs.y() - lhs.y() * rhs.x();
   return Vector3D(x,y,z);
}

