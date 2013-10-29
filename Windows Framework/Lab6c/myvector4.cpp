#include "myvector4.h"
#include <stdio.h>
#include "quaternion.h"

Vector4::Vector4(void)
{
}

Vector4::Vector4(const float _x, const float _y, const float _z, const float _w)
:x(_x),y(_y),z(_z),w(_w)
{
}

Vector4::Vector4(const Vector4 &rhs)
:x(rhs.x),y(rhs.y),z(rhs.z),w(rhs.w)
{
}

Vector4::Vector4(const Vector3 &rhs):
	x(rhs.x), y(rhs.y), z(rhs.z), w(1.0f)
{
}

Vector4::Vector4(const Quaternion &rhs):
	x(rhs.x), y(rhs.y), z(rhs.z), w(1.0f)
{
}

Vector4::~Vector4(void)
{
}

Vector4 & Vector4::operator +=(const Vector4 &rhs)
{
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	w = 1.0f;

	return *this;
}

Vector4 & Vector4::operator -=(const Vector4 &rhs)
{
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
	w = 1.0f;

	return *this;
}

float Vector4::Dot4(const Vector4 &rhs) const
{
	return (x * rhs.x + y * rhs.y + z * rhs.z + w * rhs.w);
}

float Vector4::Dot3(const Vector4 &rhs) const
{
	return (x * rhs.x + y * rhs.y + z * rhs.z);
}

Vector4 Vector4::Cross(const Vector4 &rhs) const
{
	return Vector4(y * rhs.z - z * rhs.y, z * rhs.x - x * rhs.z, x * rhs.y - y * rhs.x, 1.0f); 
}

Vector4 & Vector4::operator *=(const float s)
{
	x *= s;
	y *= s;
	z *= s;
	w  = 1.0f;

	return *this;
}

Vector4 & Vector4::operator /=(const float s)
{
	x /= s;
	y /= s;
	z /= s;
	w = 1.0f;;

	return *this;
}

float Vector4::Length() const
{
	return sqrt(x * x + y * y + z * z);
}

float Vector4::LengthSqr() const
{
	return (x * x + y * y + z * z);
}


bool Vector4::operator ==(const Vector4 & rhs) const
{
	return ((x == rhs.x) && (y == rhs.y) && (z == rhs.z) && (w == rhs.w));
}

Vector4 Vector4::Normalise()
{
	return (*this / this->Length());
}

void Vector4::NormaliseSelf()
{
	*this /= this->Length();
}

void Vector4::DumpVector4(char * s)
{
	if(s != NULL)printf("\n%f %f %f %f %s\n\n", x, y, z, w, s);
	else printf("\n%f %f %f %f\n\n", x, y, z, w);
}

void Vector4::CalcNormal(Vector4 pointA, Vector4 pointB, Vector4 pointC)
{
	Vector4 A(pointA);
	Vector4 B(pointB);
	Vector4 C(pointC);

	Vector4 AB = B - A;
	Vector4 AC = C - A;

	Vector4 Normal = AC.Cross(AB);
	Normal.NormaliseSelf();
	x = Normal.x;
	y = Normal.y;
	z = Normal.z;
	w = 1.0f;
}

std::ostream& operator<< (std::ostream& out, const Vector4& lhs)
{
	out << "(" << lhs.x << ", " << lhs.y << ", " << lhs.z << ", " << lhs.w << ")";
	return out;
}
