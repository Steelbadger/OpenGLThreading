#include "terrainmanager.h"
#include "renderer.h"
#include "terrain.h"
#include "gameobject.h"
#include "heightmap.h"
#include "material.h"

#include <iostream>
#include <process.h>


TerrainManager::TerrainManager(void) : noise(16, 500.0f, 0.41f, 80.0f, 1.155f),	terrain(CHUNKSIZE, 15.0f), water(CHUNKSIZE)
{
}


TerrainManager::~TerrainManager(void)
{
}


void TerrainManager::Initialize(RenderManager &r, NoiseObject n)
{
	std::cout << "Terrain Manager Initialising!" << std::endl;

	//  Setup our renderer pointer, the camera pointer and the noise seed
	renderer = &r;
	camera = renderer->GetCameraParent();
	noise = n;
	updateTask = NULL;
	threadCounter = 0;

	Vector2 base = (camera->GetPosition().xz() - RANGE)/CHUNKSIZE;
	std::cout << "Range Lower Extent: (" << base.u << ", " << base.v << ")" << std::endl;
	base.Floor();
	base = base * CHUNKSIZE;
	Heightmap heights;

	std::cout << "Base of Terrain Map at: (" << base.u << ", " << base.v << ")" << std::endl;
	std::cout << "Beginning Terrain Generation Loop" << std::endl;

	//  Produce and compile the default terrain shaders and textures
	defaultGround.AddTexture(Texture(Texture::DIFFUSE, "grass.tga"));
	defaultGround.AddTexture(Texture(Texture::DIFFUSE, "rockTexture.tga"));
	defaultGround.AddShader("terrain.fragmentshader");
	defaultGround.AddShader("terrain.vertexshader");
	defaultGround.AddShader("terrain.tesscontrol");
	defaultGround.AddShader("terrain.tessevaluation");
	defaultGround.Compile();

	//  Produce and compile the default water shaders and textures
	defaultWater.AddShader("water.vertexshader");
	defaultWater.AddShader("water.fragmentshader");
	defaultWater.AddShader("water.tesscontrol");
	defaultWater.AddShader("water.tessevaluation");
	defaultWater.AddTexture(Texture(Texture::DIFFUSE, "seamlessWater.tga"));
	defaultWater.Compile();
	Texture oldTex;

	//  Timers for benchmarking
	double bigTimer = clock();
	double timer = clock();

	//  Create our initial grid of terrain and water
	for(int i = 0; i < NUMCHUNKS; i++) {
		for(int j = 0; j < NUMCHUNKS; j++) {
			//  First setup the bases (the positions at which the chunks get drawn)
			bases[i*NUMCHUNKS+j] = Vector2(base.u + i*CHUNKSIZE, base.v + j * CHUNKSIZE);
			std::cout << "Generating at: (" << bases[i*NUMCHUNKS+j].u << ", " << bases[i*NUMCHUNKS+j].v << ")" << std::endl;
			timer = clock();

			//  Generate the heightmaps for those chunks
			terrainMap[bases[i*NUMCHUNKS+j].u][bases[i*NUMCHUNKS+j].v] = 
							Texture(Texture::DISPLACEMENT, heights.TBBSIMDGenerateHeightField(bases[i*NUMCHUNKS+j].u, bases[i*NUMCHUNKS+j].v, noise, CHUNKSIZE), 1024);
			std::cout << "Applying Material..." << std::endl;

			//  Attach the new heightmap to the materials
			defaultGround.ReplaceTexture(oldTex, terrainMap[bases[i*NUMCHUNKS+j].u][bases[i*NUMCHUNKS+j].v]);
			defaultWater.ReplaceTexture(oldTex, terrainMap[bases[i*NUMCHUNKS+j].u][bases[i*NUMCHUNKS+j].v]);
			oldTex = terrainMap[bases[i*NUMCHUNKS+j].u][bases[i*NUMCHUNKS+j].v];

			//  Save the new materials in the array opf materials
			materials[i*NUMCHUNKS+j] = defaultGround;
			waterMats[i*NUMCHUNKS+j] = defaultWater;
			timer = clock()-timer;
			std::cout << "Done in " << timer/CLOCKS_PER_SEC << "s" << std::endl;
		}
	}

	bigTimer = clock()-bigTimer;
	
	std::cout << "DONE TERRAIN INIT (" << bigTimer/CLOCKS_PER_SEC << "s)" << std::endl;


}

void TerrainManager::Update()
{
	//  We want to build terrain about the camera
	Vector2 base = (camera->GetPosition().xz() - RANGE)/CHUNKSIZE;

	//  Make it an integer boundary so that each base is on a chunk-border
	base.Floor();
	base = base * CHUNKSIZE;

	//  Peek at the thread counter, if there's no current running threads for this task then create one and run it
	if (threadCounter == 0) {
		threadCounter++;
		updateTask = new(tbb::task::allocate_root())UpdateTask;
		updateTask->Initialize(terrainMap, materials, waterMats, bases, noise, defaultGround, defaultWater);
		updateTask->PassMutexes(materialMutex, waterMutex, basesMutex, terrainMutex, threadCounter);
		updateTask->SetupForExecute(base);

		tbb::task::enqueue(*updateTask);
	}

}

Material TerrainManager::GetMaterial(int x , int y)
{
	materialMutex.lock_read();
	Material output = materials[x*NUMCHUNKS+y];
	materialMutex.unlock();
	return output;
}

Material TerrainManager::GetWaterMaterial(int x, int y)
{
	waterMutex.lock_read();
	Material output = waterMats[x*NUMCHUNKS+y];
	waterMutex.unlock();
	return output;
}

Vector2 TerrainManager::GetBase(int x, int y)
{
	basesMutex.lock_read();
	Vector2 output = bases[x*NUMCHUNKS+y];
	basesMutex.unlock();
	return output;
}
