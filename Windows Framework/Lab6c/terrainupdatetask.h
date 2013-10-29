#pragma once
///////////////////////// By Ross Davies /////////////////////////
//	The TBB task object that handles monitoring of the terrain	//
//	array, also creates more terrain using noise functions 		//
//	implemented using tbb parallel for							//
//////////////////////////////////////////////////////////////////
#include "myvector2.h"
#include <map>
#include "texture.h"
#include "noisegenerator.h"
#include "terrain.h"
#include "heightmap.h"
#include "material.h"
#include "TBB\task.h"
#include "TBB\spin_rw_mutex.h"
#include "TBB\atomic.h"
#include <iostream>
#include <time.h>

//  Couldn't work out where these should live
static const int RANGE = 1000;
static const int CHUNKSIZE = 600;
static const int NUMCHUNKS = (2*RANGE)/(CHUNKSIZE/2)-2;


class UpdateTask : public tbb::task
{
public:
	//  Constructor (noise has no default constructor, but this object will be replaced later)
	UpdateTask(): noise(16, 200.0f, 0.43f, 80.0f, 1.155f){};

	//  Give the task all the data and pointers it needs to do the generation job
	//  maps of terrain texture by location, default materials to build from, the 
	//  noise to use to generate the heights and the position to build around
	void Initialize(std::map<float, std::map<float, Texture> > &terrain, 
					Material *mat, Material *wMat, Vector2 *bas, NoiseObject n, 
					Material dground, Material dwater) {
		terrainMap = &terrain;
		materials = mat;
		waterMaterials = wMat;
		bases = bas;
		noise = n;
		defaultGround = dground;
		defaultWater = dwater;
	}

	//  When we're done and the task destructs decrement the threadcounter
	~UpdateTask() {
		(*threadCounter)--;
	}

	//  The task also needs access to mutexes to protect the material storage objects (so that we can stop the renderer drawing
	//  using textures that are currently being written to)
	void PassMutexes(tbb::spin_rw_mutex &material, tbb::spin_rw_mutex &water, tbb::spin_rw_mutex &basesm, tbb::spin_rw_mutex &terrainm, tbb::atomic<int> &tcounter) {
		materialMutex = material;
		waterMutex = water;
		basesMutex = basesm;
		terrainMutex = terrainm;
		threadCounter = &tcounter;
	}

	//  The point about which the terrain should be built
	void SetupForExecute(Vector2 b) {
		base = b;
	}

	//  Main task function, check the bounding box around the player (RANGE radius) for terrain, if any new terrain
	//  is needed then generate it using threaded simd functions
	tbb::task* execute() {
		//  The height generator
		Heightmap heights;

		//  Use local storage for main work, we'll swap it back into main storage later
		std::map<float, std::map<float, Texture> > oldTerrain;
		std::map<float, std::map<float, Texture> > newTerrain;
		terrainMutex.lock_read();
		oldTerrain = *terrainMap;
		terrainMutex.unlock();
		Texture oldTex;
		Material materialsBuff[NUMCHUNKS*NUMCHUNKS];
		Material waterMatsBuff[NUMCHUNKS*NUMCHUNKS];
		Vector2 basesBuff[NUMCHUNKS*NUMCHUNKS];

		//  Time the operations
		double timer = clock();
		
		//  For all chunks
		for(int i = 0; i < NUMCHUNKS; i++) {
			for(int j = 0; j < NUMCHUNKS; j++) {
				//  Find the base position of the chunk (by offsetting from the base position of the terrain as a whole)
				basesBuff[i*NUMCHUNKS+j] = Vector2(base.u + i*CHUNKSIZE, base.v + j * CHUNKSIZE);
				//  Check the base position in the map, is there already a texture for this position, if no we need
				//  to make one
				if (oldTerrain[basesBuff[i*NUMCHUNKS+j].u].count(basesBuff[i*NUMCHUNKS+j].v) == 0) {
					oldTex = oldTerrain[basesBuff[i*NUMCHUNKS+j].u][basesBuff[i*NUMCHUNKS+j].v];
					//  Tell the console what we're doing
					std::cout << "Generating New Terrain Chunk: (" << basesBuff[i*NUMCHUNKS+j].u << ", " << basesBuff[i*NUMCHUNKS+j].v << ")" << std::endl;
					timer = clock();
					//  Generate the terrain and store it to the new map of terrain
					newTerrain[basesBuff[i*NUMCHUNKS+j].u][basesBuff[i*NUMCHUNKS+j].v] = 
								Texture(Texture::DISPLACEMENT, heights.TBBSIMDGenerateHeightField(basesBuff[i*NUMCHUNKS+j].u, basesBuff[i*NUMCHUNKS+j].v, noise, CHUNKSIZE), 1024);
					timer = clock()-timer;
					std::cout << "Done in " << timer/CLOCKS_PER_SEC << "s" << std::endl;
				} else {
					//  If we already have a heightmap for that chunk then use that (store it to the new map of terrain)
					oldTex = oldTerrain[basesBuff[i*NUMCHUNKS+j].u][basesBuff[i*NUMCHUNKS+j].v];
					newTerrain[basesBuff[i*NUMCHUNKS+j].u][basesBuff[i*NUMCHUNKS+j].v] = oldTex;
				}
				//  Build the material for that chunk by using the default material, pulling the Displacement map out of it
				//  then adding it to the array of materials to be fetched by the renderer later
				materialMutex.lock_read();
				Material defaultGround = materials[i*NUMCHUNKS+j];
				materialMutex.unlock();
				defaultGround.ReplaceTexture(Texture::DISPLACEMENT, newTerrain[basesBuff[i*NUMCHUNKS+j].u][basesBuff[i*NUMCHUNKS+j].v]);
				defaultWater.ReplaceTexture(Texture::DISPLACEMENT, newTerrain[basesBuff[i*NUMCHUNKS+j].u][basesBuff[i*NUMCHUNKS+j].v]);
				oldTex = newTerrain[basesBuff[i*NUMCHUNKS+j].u][basesBuff[i*NUMCHUNKS+j].v];
				materialsBuff[i*NUMCHUNKS+j] = defaultGround;
				waterMatsBuff[i*NUMCHUNKS+j] = defaultWater;
			}
		}

		//  Do a quick-swap of the terrain maps using pointers
		terrainMutex.lock();
		newTerrain.swap(*terrainMap);
		terrainMutex.unlock();

		//  Transfer all the data from the newly calculated materials to the storage used by the manager
		//  Keep the operation thread-safe
		materialMutex.lock();
		waterMutex.lock();
		basesMutex.lock();
		for (int i = 0; i < NUMCHUNKS;i++) {
			for (int j = 0; j < NUMCHUNKS; j++) {
				materials[i*NUMCHUNKS+j] = materialsBuff[i*NUMCHUNKS+j];
				waterMaterials[i*NUMCHUNKS+j] = waterMatsBuff[i*NUMCHUNKS+j];
				bases[i*NUMCHUNKS+j] = basesBuff[i*NUMCHUNKS+j];
			}
		}
		basesMutex.unlock();
		waterMutex.unlock();
		materialMutex.unlock();

		return NULL;
	}

private:
	//  The position of the camera
	Vector2 base;
	//  Heightmap textures referenced using the base position of the chunk
	std::map<float, std::map<float, Texture> > *terrainMap;

	//  pointers to the arrays sotring these values in terrainManager
	Vector2* bases;
	Material* materials;
	Material* waterMaterials;

	//  Default materials to speed up production
	Material defaultGround;
	Material defaultWater;

	//  The noise used to generate heightmap
	NoiseObject noise;

//  MUTEXES

	tbb::spin_rw_mutex materialMutex;
	tbb::spin_rw_mutex waterMutex;
	tbb::spin_rw_mutex basesMutex;
	tbb::spin_rw_mutex terrainMutex;

//  ATOMIC COUNTER
	tbb::atomic<int>* threadCounter;
};
