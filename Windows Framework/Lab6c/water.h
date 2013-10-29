#pragma once
#include "mesh.h"
#include "myvector3.h"
#include <vector>

class Water : public Mesh
{
public:
	Water(float size);
	Water(Mesh &mesh);

	~Water(void);
	void Create();
private:
	float squareSize;
};

