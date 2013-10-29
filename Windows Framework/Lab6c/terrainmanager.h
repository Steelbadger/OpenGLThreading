#pragma once
//////////////////////--By Ross Davies--//////////////////////
/*		Class that handles creation and deletion of
		terrain as the player moves across the world.
		This class spawns a watcher thread which checks
		for the need to generate new chunks and does so
		if necessary.  Once done the watcher then
		deletes itself and signals the main application
		thread that it has died, the main thread then
		spawns another watcher thread						*/
//////////////////////////////////////////////////////////////

#include "myvector2.h"
#include <map>
#include "texture.h"
#include "noisegenerator.h"
#include "terrainupdatetask.h"
#include "terrain.h"
#include "heightmap.h"
#include "material.h"
#include "TBB\task.h"
#include "TBB\spin_rw_mutex.h"
#include "TBB\atomic.h"
#include <iostream>
#include <time.h>


class RenderManager;
class GameObject;

class TerrainManager
{
public:
	TerrainManager(void);
	~TerrainManager(void);

	//  Give the Manager the noise seed to use and the renderer to pass the new terrain into
	void Initialize(RenderManager &renderer, NoiseObject n);

	//  Check if a watcher thread needs to be made
	void Update();

	//  Get the large flat terrain mesh used to render every chunk
	Terrain GetTerrainMesh(){return terrain;}

	//  Get the large flat water mesh used to render all water
	Water GetWaterMesh(){return water;}

	//  Get the material of a given chunk
	Material GetMaterial(int i, int j);

	//  Get the water's material in a given chunk
	Material GetWaterMaterial(int i, int j);

	//  Get the number of chunks on an edge
	int GetSize(){return NUMCHUNKS;}

	//  Get the base position of the chunk at this position
	Vector2 GetBase(int i , int j);

private:
	//  Array of bases (the point at which each chunk is drawn)
	Vector2 bases[NUMCHUNKS*NUMCHUNKS];
	//  Array of materials (each chunk has a unique material as the heightmap is part of the material)
	Material materials[NUMCHUNKS*NUMCHUNKS];
	//  Array of materials (each chunk has a unique material as the heightmap is part of the material)
	Material waterMats[NUMCHUNKS*NUMCHUNKS];

	//  The default materials from which we build the specific materials
	Material defaultGround;
	Material defaultWater;

	//  The default meshes used to draw the water and terrain
	Terrain terrain;
	Water water;

	//  A 2D LUT map for finding the texture of a chunk with given position
	std::map<float, std::map<float, Texture> > terrainMap;	

	//  The pointer to the renderer that's used to draw the terrain
	RenderManager* renderer;

	//  Also need the active camera so that we know where to create the terrain around
	GameObject* camera;

	//  Store the noise we'll use
	NoiseObject noise;

	//  The pointer to the task we'll keep creating (and destroying)
	UpdateTask* updateTask;

//  MUTEXES

	tbb::spin_rw_mutex materialMutex;
	tbb::spin_rw_mutex waterMutex;
	tbb::spin_rw_mutex basesMutex;
	tbb::spin_rw_mutex terrainMutex;

//  ATOMIC COUNTER
	tbb::atomic<int> threadCounter;
};