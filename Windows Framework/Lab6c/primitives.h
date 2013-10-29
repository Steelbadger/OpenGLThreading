#pragma once
#include "mesh.h"

struct Point {
	Vector3 p;
	Vector3 n;
	Vector2 u;
	bool operator==(const Point & rhs) const {
		return ((p==rhs.p) && (n==rhs.n) && (u==rhs.u));
	}
};


class PrimitiveFactory
{
public:
	PrimitiveFactory(void);
	~PrimitiveFactory(void);

	Mesh Plane(float width, float height, int widthSubDivs, int heightSubDivs);

	Mesh SimpleInnerBox();
	Mesh UnitCube();
	Mesh LSphere(int lats, int longs);
	Mesh GSphere(int resolution);
	Mesh SubDivide(Mesh m);
	Mesh SubDivide(Mesh m, int res);
	Mesh Spherize(Mesh m);
	Mesh CubeSphere(int resolution);
	Mesh InwardCubeSphere(int resolution);
private:
	Point AveragePoints(Point p1, Point p2);
};

