#pragma once

class Vector2
{
public:
	Vector2(void);
	Vector2(float _u, float _v);
	~Vector2(void);

	bool operator==(const Vector2 & rhs) const;
	void Floor();

	float u, v;
};

inline Vector2 operator * (const Vector2 &v,
						   const float &s)
{
	return Vector2(v.u * s, v.v * s);
}

inline Vector2 operator / (const Vector2 &v,
						   const float &s)
{
	return Vector2(v.u / s, v.v / s);
}

inline Vector2 operator + (const Vector2 &v,
						   const float &s)
{
	return Vector2(v.u + s, v.v + s);
}

inline Vector2 operator - (const Vector2 &v,
						   const float &s)
{
	return Vector2(v.u - s, v.v - s);
}

inline Vector2 operator - (const Vector2 &v,
						   const Vector2 &s)
{
	return Vector2(v.u - s.u, v.v - s.v);
}


inline Vector2 operator + (const Vector2 &v,
						   const Vector2 &s)
{
	return Vector2(v.u + s.u, v.v + s.v);
}

