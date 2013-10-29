#include "mesh.h"

#include "gameobject.h"
#include "tex.h"

#include <string>
#include <iostream>

int Mesh::IDCOUNTER = 0;
std::map<int, Mesh*> Mesh::IdToMeshMap;

Mesh* Mesh::GetMeshPointer(int uniqueID)
{
	if (IdToMeshMap.count(uniqueID)) {
		return IdToMeshMap[uniqueID];
	} else {
		return NULL;
	}
}

Mesh::Mesh():
	uniqueID(IDCOUNTER++)
{
	IdToMeshMap[uniqueID] = this;
}

Mesh::Mesh(const Mesh& m):
	uniqueID(IDCOUNTER++),
	meshPath(m.meshPath),
	parent(m.parent),
	transparency(m.transparency),
	verts(m.verts),
	normals(m.normals),
	uvs(m.uvs),
	index(m.index),
	numVerts(m.numVerts),
	successfullBuild(m.successfullBuild),
	material(m.material)
{
	IdToMeshMap[uniqueID] = this;
}

Mesh::Mesh(const char* mPath, GameObject* p):
	uniqueID(IDCOUNTER++)
{
	meshPath = mPath;
	parent = p;
	IdToMeshMap[uniqueID] = this;
	transparency = false;
	LoadMesh(meshPath.c_str());
}

Mesh::Mesh(const char* mPath):
	uniqueID(IDCOUNTER++)
{
	meshPath = mPath;
	parent = NULL;
	IdToMeshMap[uniqueID] = this;
	transparency = false;
	LoadMesh(meshPath.c_str());
}

Mesh::Mesh(std::vector<Vector3> v, std::vector<Vector3> n, std::vector<Vector2> u):
	uniqueID(IDCOUNTER++)
{
	parent = NULL;
	IdToMeshMap[uniqueID] = this;
	transparency = false;
	verts = v;
	normals = n;
	uvs = u;
	for (int i = 0; i < verts.size(); i++) {
		index.push_back(i);
	}
	numVerts = index.size();
}

Mesh::Mesh(std::vector<Vector3> v, std::vector<Vector3> n, std::vector<Vector2> u, std::vector<unsigned int> i):
	uniqueID(IDCOUNTER++)
{
	parent = NULL;
	IdToMeshMap[uniqueID] = this;
	transparency = false;
	verts = v;
	normals = n;
	uvs = u;
	index = i;
	numVerts = index.size();
}

Mesh::~Mesh(void)
{
	IdToMeshMap.erase(uniqueID);
}

bool Mesh::LoadMesh(const char* path)
{
	std::string fn = path;
	if(fn.substr(fn.find_last_of(".") + 1) == "obj") {
		return LoadObj(path);
	} else {
		return false;
	}
}

bool Mesh::LoadObj(const char* path)
{
	std::vector<unsigned int> vertIndices, uvIndices, normalIndices;
	std::vector<Vector3> tempVerts;
	std::vector<Vector3> tempNormals;
	std::vector<Vector2> tempUVs;

	FILE * file = fopen(path, "r");
	if( file == NULL ){
		std::cout << "Cannot Open File: " << path << std::endl;
		return false;
	}

	while(true){

		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

		// else : parse lineHeader
		
		if ( strcmp( lineHeader, "v" ) == 0 ){
			Vector3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
			tempVerts.push_back(vertex);
		} else if ( strcmp( lineHeader, "vt" ) == 0 ){
			Vector2 uv;
			fscanf(file, "%f %f\n", &uv.u, &uv.v );
			tempUVs.push_back(uv);
		} else if ( strcmp( lineHeader, "vn" ) == 0 ){
			Vector3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
			tempNormals.push_back(normal);
		} else if ( strcmp( lineHeader, "f" ) == 0 ){
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
			if (matches != 9){
				std::cout << "Cannot Read File: " << path << std::endl;
				fclose(file);
				return false;
			}
			vertIndices.push_back(vertexIndex[0]);
			vertIndices.push_back(vertexIndex[1]);
			vertIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		} else {
			// Probably a comment, eat up the rest of the line
			char stuff[1000];
			fgets(stuff, 1000, file);
		}

	}

	// For each vertex of each triangle
	for( unsigned int i=0; i<vertIndices.size(); i++ ){

		// Get the indices of its attributes
		unsigned int vertexIndex = vertIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];
		
		// Get the attributes thanks to the index
		Vector3 vertex = tempVerts[vertexIndex-1];
		Vector2 uv = tempUVs[uvIndex-1];
		Vector3 normal = tempNormals[normalIndex-1];
		
		// Put the attributes in buffers
		verts.push_back(vertex);
		uvs.push_back(uv);
		normals.push_back(normal);
		index.push_back(i);
	}
	fclose(file);
	numVerts = vertIndices.size();

	return true;
}

void Mesh::DeleteVertexData()
{
	verts.clear();
	normals.clear();
	uvs.clear();
	index.clear();
}

void Mesh::ReverseNormals()
{
	for (int i = 0; i < normals.size(); i++) {
		normals[i] = normals[i]*-1;
	}
}

Mesh& Mesh::operator = (const Mesh& m)
{
	meshPath = m.meshPath;
	parent = m.parent;
	transparency = m.transparency;
	verts = m.verts;
	normals = m.normals;
	uvs = m.uvs;
	index = m.index;
	numVerts = m.numVerts;
	successfullBuild = m.successfullBuild;
	material = m.material;

	return *this;
}
