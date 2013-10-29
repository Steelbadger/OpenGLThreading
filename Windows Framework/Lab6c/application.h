#pragma once


#include "mywindow.h"
#include "noisegenerator.h"
#include "player.h"
#include "primitives.h"
#include "renderer.h"
#include "staticobject.h"
#include "Terrain.h"
#include "terrainmanager.h"

#include "my4x4matrix.h"



#include <windows.h>
#include <stdio.h>
#include <mmsystem.h>


class Application
{
public:
	Application(void);
	~Application(void);

	void Initialize(HINSTANCE hInstance);
	void MainLoop();

	void Debug();

private:
	void InitialiseScene();
	void CrossProductBenchmark();
	void PerlinSimplexBenchmark();
	void SIMDThreadingBenchmark();

	WindowWizard window;
	Player player;

	double lastTime;
	int nbFrames;
	double currentTime;
	bool wireframe;
	bool framerateLogging;
	bool culling;

	const int gridSize;
	NoiseObject myNoise;
	PrimitiveFactory meshGenerator;
	RenderManager renderer;
	TerrainManager testTerrain;
	GameObject sunParent;
	Mesh skybox;

	StaticObject testObject;
	StaticObject boat;
};

