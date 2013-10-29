#include "primitives.h"

#include <math.h>

PrimitiveFactory::PrimitiveFactory(void)
{
}


PrimitiveFactory::~PrimitiveFactory(void)
{
}

Mesh PrimitiveFactory::Plane(float width, float height, int widthSubDivs, int heightSubDivs)
{
	std::vector<Vector3> verts;
	std::vector<Vector3> normals;
	std::vector<Vector2> texCoords;
	std::vector<unsigned int> index;

	float widthStep = width/(widthSubDivs-1);
	float heightStep = height/(heightSubDivs-1);

	for (int i = 0; i < heightSubDivs; i++) {
		for (int j = 0; j < widthSubDivs; j++) {
			verts.push_back(Vector3(i*widthStep-width/2, 0, j*heightStep-height/2));
			Vector3 normalA = Vector3(0,1,0);
			normals.push_back(normalA);
			texCoords.push_back(Vector2(i*widthStep/width, j*heightStep/height));
		}
	}

	for (int i = 0; i < heightSubDivs-1; i++) {
		for (int j = 0; j < widthSubDivs-1; j++) {
			index.push_back(i+j*widthSubDivs);
			index.push_back(1+i+(1+j)*widthSubDivs);
			index.push_back(i+(1+j)*widthSubDivs);
			index.push_back(1+i+(1+j)*widthSubDivs);
			index.push_back(i+j*widthSubDivs);
			index.push_back(1+i+j*widthSubDivs);
		}
	}

	return Mesh(verts, normals, texCoords, index);
}

Mesh PrimitiveFactory::SimpleInnerBox()
{
	std::vector<Vector3> verts;
	std::vector<Vector3> normals;
	std::vector<Vector2> texCoords;

	std::vector<Vector3> points;
	std::vector<Vector3> faceNormals;
	std::vector<Vector2> uvPoints;

	for(int i = 0; i < 2; i++) {
		for(int j = 0; j < 2; j++) {
			for (int k = 0; k < 2; k++) {
				points.push_back(Vector3(-0.5+i, -0.5+j, -0.5+k));
			}
		}
	}

	faceNormals.push_back(Vector3(1.0, 0, 0));
	faceNormals.push_back(Vector3(-1.0, 0, 0));
	faceNormals.push_back(Vector3(0, 1.0, 0));
	faceNormals.push_back(Vector3(0, -1.0, 0));
	faceNormals.push_back(Vector3(0, 0, 1.0));
	faceNormals.push_back(Vector3(0, 0, -1.0));

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 5; j++) {
			uvPoints.push_back(Vector2(0.25*j, 0.25*i));
		}
	}


	//  Face 1 - Triangle 1
	verts.push_back(points[1]);
	verts.push_back(points[5]);
	verts.push_back(points[0]);
	for (int i = 0; i < 3; i++) {
		normals.push_back(faceNormals[2]);
	}
	texCoords.push_back(uvPoints[5]);
	texCoords.push_back(uvPoints[10]);
	texCoords.push_back(uvPoints[6]);



	//  Face 1 - Triangle 2
	verts.push_back(points[5]);
	verts.push_back(points[4]);
	verts.push_back(points[0]);

	for (int i = 0; i < 3; i++) {
		normals.push_back(faceNormals[2]);
	}
	texCoords.push_back(uvPoints[10]);
	texCoords.push_back(uvPoints[11]);
	texCoords.push_back(uvPoints[6]);


	//  Face 2 - Triangle 1
	verts.push_back(points[5]);
	verts.push_back(points[7]);
	verts.push_back(points[4]);

	for (int i = 0; i < 3; i++) {
		normals.push_back(faceNormals[1]);
	}
	texCoords.push_back(uvPoints[10]);
	texCoords.push_back(uvPoints[15]);
	texCoords.push_back(uvPoints[11]);



	//  Face 2 - Triangle 2
	verts.push_back(points[7]);
	verts.push_back(points[6]);
	verts.push_back(points[4]);

	for (int i = 0; i < 3; i++) {
		normals.push_back(faceNormals[1]);
	}
	texCoords.push_back(uvPoints[15]);
	texCoords.push_back(uvPoints[16]);
	texCoords.push_back(uvPoints[11]);


	//  Face 3 - Triangle 1
	verts.push_back(points[3]);
	verts.push_back(points[1]);
	verts.push_back(points[2]);

	for (int i = 0; i < 3; i++) {
		normals.push_back(faceNormals[0]);
	}
	texCoords.push_back(uvPoints[0]);
	texCoords.push_back(uvPoints[5]);
	texCoords.push_back(uvPoints[1]);


	//  Face 3 - Triangle 2
	verts.push_back(points[1]);
	verts.push_back(points[0]);
	verts.push_back(points[2]);

	for (int i = 0; i < 3; i++) {
		normals.push_back(faceNormals[0]);
	}
	texCoords.push_back(uvPoints[5]);
	texCoords.push_back(uvPoints[6]);
	texCoords.push_back(uvPoints[1]);


	//  Face 4 - Triangle 1
	verts.push_back(points[0]);
	verts.push_back(points[4]);
	verts.push_back(points[2]);

	for (int i = 0; i < 3; i++) {
		normals.push_back(faceNormals[4]);
	}
	texCoords.push_back(uvPoints[6]);
	texCoords.push_back(uvPoints[11]);
	texCoords.push_back(uvPoints[7]);


	//  Face 4 - Triangle 2
	verts.push_back(points[4]);
	verts.push_back(points[6]);
	verts.push_back(points[2]);

	for (int i = 0; i < 3; i++) {
		normals.push_back(faceNormals[4]);
	}
	texCoords.push_back(uvPoints[11]);
	texCoords.push_back(uvPoints[12]);
	texCoords.push_back(uvPoints[7]);


	//  Face 5 - Triangle 1
	verts.push_back(points[2]);
	verts.push_back(points[6]);
	verts.push_back(points[3]);

	for (int i = 0; i < 3; i++) {
		normals.push_back(faceNormals[3]);
	}
	texCoords.push_back(uvPoints[7]);
	texCoords.push_back(uvPoints[12]);
	texCoords.push_back(uvPoints[8]);


	//  Face 5 - Triangle 2
	verts.push_back(points[6]);
	verts.push_back(points[7]);
	verts.push_back(points[3]);

	for (int i = 0; i < 3; i++) {
		normals.push_back(faceNormals[3]);
	}
	texCoords.push_back(uvPoints[12]);
	texCoords.push_back(uvPoints[13]);
	texCoords.push_back(uvPoints[8]);


	//  Face 6 - Triangle 1
	verts.push_back(points[3]);
	verts.push_back(points[7]);
	verts.push_back(points[1]);

	for (int i = 0; i < 3; i++) {
		normals.push_back(faceNormals[5]);
	}
	texCoords.push_back(uvPoints[8]);
	texCoords.push_back(uvPoints[13]);
	texCoords.push_back(uvPoints[9]);


	//  Face 6 - Triangle 2
	verts.push_back(points[7]);
	verts.push_back(points[5]);
	verts.push_back(points[1]);

	for (int i = 0; i < 3; i++) {
		normals.push_back(faceNormals[5]);
	}
	texCoords.push_back(uvPoints[13]);
	texCoords.push_back(uvPoints[14]);
	texCoords.push_back(uvPoints[9]);


	return Mesh(verts, normals, texCoords);
}

Mesh PrimitiveFactory::UnitCube()
{
	std::vector<Vector3> verts;
	std::vector<Vector3> normals;
	std::vector<Vector2> texCoords;

	std::vector<Vector3> points;
	std::vector<Vector3> faceNormals;
	std::vector<Vector2> uvPoints;

	for(int i = 0; i < 2; i++) {
		for(int j = 0; j < 2; j++) {
			for (int k = 0; k < 2; k++) {
				points.push_back(Vector3(-0.5+i, -0.5+j, -0.5+k));
			}
		}
	}

	faceNormals.push_back(Vector3(-1.0, 0, 0));
	faceNormals.push_back(Vector3(1.0, 0, 0));
	faceNormals.push_back(Vector3(0, -1.0, 0));
	faceNormals.push_back(Vector3(0, 1.0, 0));
	faceNormals.push_back(Vector3(0, 0, -1.0));
	faceNormals.push_back(Vector3(0, 0, 1.0));

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 5; j++) {
			uvPoints.push_back(Vector2(0.25*j, 0.25*i));
		}
	}


	//  Face 1 - Triangle 1
	verts.push_back(points[1]);
	verts.push_back(points[0]);
	verts.push_back(points[5]);

	for (int i = 0; i < 3; i++) {
		normals.push_back(faceNormals[2]);
	}
	texCoords.push_back(uvPoints[5]);
	texCoords.push_back(uvPoints[6]);
	texCoords.push_back(uvPoints[10]);




	//  Face 1 - Triangle 2
	verts.push_back(points[5]);
	verts.push_back(points[0]);
	verts.push_back(points[4]);

	for (int i = 0; i < 3; i++) {
		normals.push_back(faceNormals[2]);
	}
	texCoords.push_back(uvPoints[10]);
	texCoords.push_back(uvPoints[6]);
	texCoords.push_back(uvPoints[11]);



	//  Face 2 - Triangle 1
	verts.push_back(points[5]);
	verts.push_back(points[4]);
	verts.push_back(points[7]);

	for (int i = 0; i < 3; i++) {
		normals.push_back(faceNormals[1]);
	}
	texCoords.push_back(uvPoints[10]);
	texCoords.push_back(uvPoints[11]);
	texCoords.push_back(uvPoints[15]);




	//  Face 2 - Triangle 2
	verts.push_back(points[7]);
	verts.push_back(points[4]);
	verts.push_back(points[6]);

	for (int i = 0; i < 3; i++) {
		normals.push_back(faceNormals[1]);
	}
	texCoords.push_back(uvPoints[15]);
	texCoords.push_back(uvPoints[11]);
	texCoords.push_back(uvPoints[16]);


	//  Face 3 - Triangle 1
	verts.push_back(points[3]);
	verts.push_back(points[2]);
	verts.push_back(points[1]);

	for (int i = 0; i < 3; i++) {
		normals.push_back(faceNormals[0]);
	}
	texCoords.push_back(uvPoints[0]);
	texCoords.push_back(uvPoints[1]);
	texCoords.push_back(uvPoints[5]);


	//  Face 3 - Triangle 2
	verts.push_back(points[1]);
	verts.push_back(points[2]);
	verts.push_back(points[0]);

	for (int i = 0; i < 3; i++) {
		normals.push_back(faceNormals[0]);
	}
	texCoords.push_back(uvPoints[5]);
	texCoords.push_back(uvPoints[1]);
	texCoords.push_back(uvPoints[6]);


	//  Face 4 - Triangle 1
	verts.push_back(points[0]);
	verts.push_back(points[2]);
	verts.push_back(points[4]);

	for (int i = 0; i < 3; i++) {
		normals.push_back(faceNormals[4]);
	}
	texCoords.push_back(uvPoints[6]);
	texCoords.push_back(uvPoints[7]);
	texCoords.push_back(uvPoints[11]);


	//  Face 4 - Triangle 2
	verts.push_back(points[4]);
	verts.push_back(points[2]);
	verts.push_back(points[6]);

	for (int i = 0; i < 3; i++) {
		normals.push_back(faceNormals[4]);
	}
	texCoords.push_back(uvPoints[11]);
	texCoords.push_back(uvPoints[7]);
	texCoords.push_back(uvPoints[12]);


	//  Face 5 - Triangle 1
	verts.push_back(points[2]);
	verts.push_back(points[3]);
	verts.push_back(points[6]);

	for (int i = 0; i < 3; i++) {
		normals.push_back(faceNormals[3]);
	}
	texCoords.push_back(uvPoints[7]);
	texCoords.push_back(uvPoints[8]);
	texCoords.push_back(uvPoints[12]);


	//  Face 5 - Triangle 2
	verts.push_back(points[6]);
	verts.push_back(points[3]);
	verts.push_back(points[7]);

	for (int i = 0; i < 3; i++) {
		normals.push_back(faceNormals[3]);
	}
	texCoords.push_back(uvPoints[12]);
	texCoords.push_back(uvPoints[8]);
	texCoords.push_back(uvPoints[13]);


	//  Face 6 - Triangle 1
	verts.push_back(points[3]);
	verts.push_back(points[1]);
	verts.push_back(points[7]);

	for (int i = 0; i < 3; i++) {
		normals.push_back(faceNormals[5]);
	}
	texCoords.push_back(uvPoints[8]);
	texCoords.push_back(uvPoints[9]);
	texCoords.push_back(uvPoints[13]);

	//  Face 6 - Triangle 2
	verts.push_back(points[7]);
	verts.push_back(points[1]);
	verts.push_back(points[5]);

	for (int i = 0; i < 3; i++) {
		normals.push_back(faceNormals[5]);
	}
	texCoords.push_back(uvPoints[13]);
	texCoords.push_back(uvPoints[9]);
	texCoords.push_back(uvPoints[14]);



	return Mesh(verts, normals, texCoords);
}

Mesh PrimitiveFactory::LSphere(int lats, int longs)
{
	const float pi = 3.14159265359;

	std::vector<Vector3> verts;
	std::vector<Vector3> normals;
	std::vector<Vector2> texCoords;
	std::vector<unsigned int> index;


	//  Find the angles between consecutive radii
	float deltaTheta = pi/(lats-1);
	float deltaGamma = 2*pi/(longs-1);

	//  Add the Poles
	verts.push_back(Vector3(0, 1, 0));
	verts.push_back(Vector3(0, -1, 0));


	//  Add the other points
	//  Outer Loop for latitudes
	//  Inner Loop for Longitudes
	for(float Theta = deltaTheta; Theta < (pi - deltaTheta); Theta+=deltaTheta) {
		for(float Gamma = 0; Gamma < 2*pi; Gamma+=deltaGamma) {
			float sinTheta = sin(Theta);
			float cosTheta = cos(Theta);
			float sinGamma = sin(Gamma);
			float cosGamma = cos(Gamma);
			Vector3 point(sinTheta*cosGamma, sinTheta*sinGamma, cosTheta);
			verts.push_back(point);
			texCoords.push_back(Vector2(Theta/(pi), Gamma/(pi*2)));
		}
	}

	normals = verts;

	//  Now to index the sphere

	//  First do two loops, as the area from the pole to the first line of latitude is a unique case

	for (int i = 0; i < lats*2; i++) {
		index.push_back(0);
		index.push_back(2+i*lats);
		index.push_back(2+(i+1)*lats);
	}

	return Mesh(verts, normals, texCoords, index);
}

Mesh PrimitiveFactory::SubDivide(Mesh m)
{
	//  First turn into a non indexed vertex array
	std::vector<Point> points;

	std::vector<unsigned int>::iterator indexIt;
	std::vector<unsigned int> index = m.GetIndex();

	std::vector<Vector3> verts = m.GetVerts();
	std::vector<Vector3> normals = m.GetNormals();
	std::vector<Vector2> uvs = m.GetUVs();

	for(indexIt = index.begin(); indexIt != index.end(); indexIt++) {
		Point temp;
		temp.p = verts[*indexIt];
		temp.n = normals[*indexIt];
		temp.u = uvs[*indexIt];
		points.push_back(temp);
	}

	//  Now create a bunch of new faces (non indexed)
	std::vector<Point> newPoints;

	for(int i = 0; i < points.size(); i+=3) {
		Point p1 = points[i];
		Point p2 = points[i+1];
		Point p3 = points[i+2];
		Point p12 = AveragePoints(p1, p2);
		Point p13 = AveragePoints(p1, p3);
		Point p23 = AveragePoints(p2, p3);

		//  Push four new triangles to the newPoints array

		newPoints.push_back(p1);
		newPoints.push_back(p12);
		newPoints.push_back(p13);

		newPoints.push_back(p2);
		newPoints.push_back(p23);
		newPoints.push_back(p12);

		newPoints.push_back(p12);
		newPoints.push_back(p23);
		newPoints.push_back(p13);

		newPoints.push_back(p13);
		newPoints.push_back(p23);
		newPoints.push_back(p3);
	}

	//  Now pull the data out into the 3 arrays needed for the mesh constructor

	std::vector<Point>::iterator pointIt;

	verts.clear();
	normals.clear();
	uvs.clear();
	
	for(pointIt = newPoints.begin(); pointIt != newPoints.end(); pointIt++) {
		verts.push_back((*pointIt).p);
		normals.push_back((*pointIt).n);
		uvs.push_back((*pointIt).u);		
	}

	return Mesh(verts, normals, uvs);
}

Mesh PrimitiveFactory::SubDivide(Mesh m, int count)
{
	if (count == 0) {
		return m;
	} else {
		return SubDivide(SubDivide(m), --count);
	}
}

Point PrimitiveFactory::AveragePoints(Point p1, Point p2)
{
	Point out;
	out.p = (p1.p+p2.p)/2;
	out.n = (p1.n+p2.n)/2;
	out.u = (p1.u+p2.u)/2;

	return out;
}

Mesh PrimitiveFactory::Spherize(Mesh m)
{
	//  First turn into a non indexed vertex array
	std::vector<Point> points;

	std::vector<unsigned int>::iterator indexIt;
	std::vector<unsigned int> index = m.GetIndex();

	std::vector<Vector3> verts = m.GetVerts();
	std::vector<Vector3> normals = m.GetNormals();
	std::vector<Vector2> uvs = m.GetUVs();

	for(indexIt = index.begin(); indexIt != index.end(); indexIt++) {
		Point temp;
		temp.p = verts[*indexIt];
		temp.n = normals[*indexIt];
		temp.u = uvs[*indexIt];
		points.push_back(temp);
	}

	std::vector<Point>::iterator pointIt;

	verts.clear();
	normals.clear();
	uvs.clear();
	
	for(pointIt = points.begin(); pointIt != points.end(); pointIt++) {
		verts.push_back((*pointIt).p.Normalize());
		normals.push_back((*pointIt).p.Normalize());
		uvs.push_back((*pointIt).u);		
	}

	return Mesh(verts, normals, uvs);
}

Mesh PrimitiveFactory::CubeSphere(int resolution)
{
	Mesh m(SubDivide(UnitCube(), resolution));

	return Mesh(Spherize(m));
}

Mesh PrimitiveFactory::InwardCubeSphere(int resolution)
{
	Mesh m(Spherize(SubDivide(SimpleInnerBox(), resolution)));
	m.ReverseNormals();

	return Mesh(m);
}