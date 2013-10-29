#pragma once
///////////////////////// By Ross Davies /////////////////////////
//	Heightmap generation class, can use varying functions to	//
//	generate a perlin or simplex noise based heightmap using	//
//	threads or SIMD as requested								//
//////////////////////////////////////////////////////////////////
#include "noisegenerator.h"
#include "glew.h"
#include <gl\GL.h>

//  A structure for holding the variables needed for win32 threading functions
struct ThreadData
{
	ThreadData():n(12, 500.0f, 0.41f, 70.0f, -1563.155f){}
	ThreadData(GLushort* p, int baseX, int baseY, int size, float mbaseX, float mbaseY, float msize, int ssize, NoiseObject noise) :
		start(p), imageBaseX(baseX), imageBaseY(baseY), imageSize(size), mapBaseX(mbaseX), mapBaseY(mbaseY),	mapSize(msize),
			sectionSize(ssize),	n(noise) {}
	GLushort* start;
	int imageBaseX;
	int imageBaseY;
	int imageSize;
	float mapBaseX;
	float mapBaseY;
	float mapSize;
	int sectionSize;
	NoiseObject n;
};


class Heightmap
{
public:
	Heightmap(void);
	~Heightmap(void);

	//  Heightmap texture generation functions using different methods
	unsigned int GenerateHeightmap(float x, float y, NoiseObject n, float square);
	unsigned short* GenerateHeightField(float x, float y, NoiseObject n, float square);
	unsigned short* TBBGenerateHeightField(float x, float y, NoiseObject n, float square);
	unsigned short* TBBSIMDGenerateHeightField(float x, float y, NoiseObject n, float square);

	//  Benchmarking functions that generate heightmaps and discard them
	void GenHeightsSIMD(float x, float y, NoiseObject n, float square);
	void GenHeightsLinear(float x, float y, NoiseObject n, float square);
	void GenHeightsTBBSIMD(float x, float y, NoiseObject n, float square);
	
private:
	void write_tga(const char *filename, int size, unsigned char* base);
	void write_tga(const char *filename, int size, unsigned short* base);
	//  Win32 threading function for generating a sub-section of a heightmap
	static unsigned __stdcall GenerateSection(void *data);

	static const int size = 1024;
};

