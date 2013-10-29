#include "myvector2.h"
#include <math.h>


Vector2::Vector2(void)
{
}

Vector2::Vector2(float _u, float _v):
	u(_u), v(_v)
{
}

Vector2::~Vector2(void)
{
}

bool Vector2::operator==(const Vector2 &rhs) const
{
	return ((u==rhs.u)&&(v==rhs.v));
}

void Vector2::Floor()
{
	u = floor(u);
	v = floor(v);
}