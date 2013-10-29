#pragma once

class Vector4;

class Vector3
{
public:
	Vector3(void);
	Vector3(const float x, const float y, const float z);
	Vector3(const Vector3 & rhs);
	Vector3(const Vector4 & rhs);
	~Vector3(void);


	bool operator==(const Vector3 & rhs) const;
	Vector3 & operator+=(const Vector3 & rhs);

	Vector3 Normalize();
	float Length();

	float x, y, z;
};

inline Vector3 operator * (const Vector3 &v,
						   const float &s)
{
	return Vector3(v.x * s, v.y * s, v.z * s);
}

inline Vector3 operator / (const Vector3 &v,
						   const float &s)
{
	return Vector3(v.x / s, v.y / s, v.z / s);
}

inline Vector3 operator + (const Vector3 &v,
						   const float &s)
{
	return Vector3(v.x + s, v.y + s, v.z + s);
}

inline Vector3 operator - (const Vector3 &v,
						   const float &s)
{
	return Vector3(v.x - s, v.y - s, v.z - s);
}

inline Vector3 operator - (const Vector3 &v,
						   const Vector3 &s)
{
	return Vector3(v.x - s.x, v.y - s.y, v.z - s.z);
}


inline Vector3 operator + (const Vector3 &v,
						   const Vector3 &s)
{
	return Vector3(v.x + s.x, v.y + s.y, v.z + s.z);
}


