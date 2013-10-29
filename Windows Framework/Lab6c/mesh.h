#pragma once

#include "material.h"
#include "myvector3.h"
#include "myvector2.h"

#include <map>
#include <vector>
#include <windows.h>
#include <stdio.h>
#include <string>

class GameObject;


class Mesh
{
public:
	Mesh(const char* meshPath, GameObject* parent);
	Mesh(const char* meshPath);
	Mesh(std::vector<Vector3> verts, std::vector<Vector3> normals, std::vector<Vector2> uvs);
	Mesh(std::vector<Vector3> verts, std::vector<Vector3> normals, std::vector<Vector2> uvs, std::vector<unsigned int> index);
	Mesh();
	Mesh(const Mesh& mesh);
	~Mesh(void);

	void AttachMaterial(Material m){material = m; material.Compile();}
	Material GetMaterial(){return material;}

	int GetUniqueID(){return uniqueID;}
	void GetNewUniqueID();
	void SetParent(GameObject* p) {parent = p;}
	GameObject* GetParentPointer(){return parent;}
	bool IsTransparent(){return transparency;}
	void SetTransparent(bool t) {transparency = t;}
	Vector3* GetVertexArrayBase(){return &verts[0];}
	Vector3* GetNormalArrayBase(){return &normals[0];}
	Vector2* GetUVArrayBase(){return &uvs[0];}
	unsigned int* GetIndexArrayBase(){return &index[0];}
	std::string GetMeshSourceFilePath(){return meshPath;}
	int GetNumberOfVerts(){return numVerts;}
	int GetSizeOfVerts() {return verts.size()*sizeof(float)*3;}
	int GetSizeOfNormals() {return normals.size()*sizeof(float)*3;}
	int GetSizeOfUVs() {return uvs.size()*sizeof(float)*2;}
	int GetIndexLength(){return index.size();}
	void ReverseNormals();

	std::vector<Vector3> GetVerts(){return verts;}
	std::vector<Vector3> GetNormals(){return normals;}
	std::vector<Vector2> GetUVs(){return uvs;}
	std::vector<unsigned int> GetIndex(){return index;}

	static Mesh* GetMeshPointer(int uniqueID);

	void DeleteVertexData();

	Mesh& operator=(const Mesh& m);

protected:
	std::vector<Vector3> verts;
	std::vector<Vector3> normals;
	std::vector<Vector2> uvs;
	std::vector<unsigned int> index;

	bool transparency;

private:
	bool LoadMesh(const char* path);
	bool LoadObj(const char* path);

	int numVerts;
	const int uniqueID;

	GameObject* parent;
	std::string meshPath;

	Material material;

	bool successfullBuild;

	static int IDCOUNTER;
	static std::map<int, Mesh*> IdToMeshMap;
};