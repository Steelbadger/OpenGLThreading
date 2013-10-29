#pragma once

#include "mesh.h"
#include "noisegenerator.h"
#include "water.h"

class Terrain : public Mesh
{
public:
	Terrain(float size, float resolution);
	Terrain(Mesh &mesh);
	Terrain();
	~Terrain(void);

	void Create();

	float GetStep(){return step;}
	float GetSize(){return squareSize;}

private:
	float squareSize;
	float resolution;
	float step;
};