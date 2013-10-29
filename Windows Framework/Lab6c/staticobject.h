#pragma once
///////////////////////// By Ross Davies /////////////////////////
//	A minimalistic extension of GameObject to provide direct	//
//	access to n meshes (this is a nasty paste-over-the-cracks	//
//	fix for Meshes attached to GameObjects going out of scope	//
//////////////////////////////////////////////////////////////////
#include "gameobject.h"
#include "mesh.h"

class StaticObject :
	public GameObject
{
public:
	StaticObject(void);
	~StaticObject(void);

	void AttachMesh(Mesh m) {m.SetParent(this); meshes.push_back(m);}
	std::vector<Mesh> GetMeshes(){return meshes;}
	virtual Mesh* GetMesh(){return &meshes[0];}

private:
	std::vector<Mesh> meshes;
};

