#include "Terrain.h"

#include "myvector4.h"
#include "my4x4matrix.h"
#include "noisegenerator.h"


#include <time.h>
#include <string>
#include <iostream>
#include <algorithm>


Terrain::Terrain()
{
}

Terrain::Terrain(float s, float r):
		squareSize(s),
			resolution(r)
{
	Create();
}

Terrain::Terrain(Mesh &m) :
	Mesh(m)
{

	std::vector<Vector3>::iterator it1 = verts.begin();
	std::vector<Vector3>::iterator it2 = verts.end();

	it2--;
	float lower = (*it1).x;
	float higher = (*it2).x;

	squareSize = higher - lower;
	float size = sqrt(float(verts.size()));
	step = squareSize/(size - 1);
	resolution = squareSize / size;
}


Terrain::~Terrain(void)
{
}

void Terrain::Create()
{

	float height = 0;

	int size = squareSize/resolution;
	step = squareSize/(size-1);

//	double myTime = clock();

	for (float i = 0; i < size; i++) {
		for (float j = 0; j < size; j++) {
			verts.push_back(Vector3(i*step, 0, j*step));
			Vector3 normalA = Vector3(0,0,0);
			normals.push_back(normalA);
			uvs.push_back(Vector2(i*step/4, j*step/4));
		}
	}

	for (int i = 0; i < size-1; i++) {
		for (int j = 0; j < size-1; j++) {
			index.push_back(i+j*size);
			index.push_back(1+i+(1+j)*size);
			index.push_back(i+(1+j)*size);
			index.push_back(1+i+(1+j)*size);
			index.push_back(i+j*size);
			index.push_back(1+i+j*size);
		}
	}

//	myTime = clock()-myTime;

//	std::cout << "Number of Triangles in Terrain Mesh: " << index.size()/3 << std::endl;
//	std::cout << "Generation Time: " << myTime/CLOCKS_PER_SEC << "s" << std::endl;
}
