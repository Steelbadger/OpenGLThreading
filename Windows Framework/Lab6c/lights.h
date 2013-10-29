#pragma once
///////////////////////// By Ross Davies /////////////////////////
//	A light is a special kind of GameObject that has additional	//
//	functionality for loading into the shaders, to provide		//
//	Lighting													//
//////////////////////////////////////////////////////////////////

#include "gameobject.h"

class Matrix4x4;
class Vector4;
class Vector3;

//  This struct is returned by the class when requested and is what
//  is used to represent a light within the shaders
struct Light
{
	float position[4];
	float colour[4];
	int type;
};

class LightSource : public GameObject
{
public:
	//  Lights must be one of three types
	enum Type{POINT, DIRECTIONAL, SPOT};

	LightSource(Type);
	~LightSource(void);

	//  Send light data to a compact struct for passing to the shaders
	Light GetLightAsStruct(Matrix4x4 view);

	void SetColour(float r, float g, float b);
	void SetAmbient(float brightness);
	void SetBeamAngle(float angle);
private:
	Vector3 colour;
	float ambient;
	Type type;
	float angle;

	//  Helper functions for generating the Light (for shaders) struct
	void CalculateDirectional(Matrix4x4 view, Light &out);
	void CalculatePoint(Matrix4x4 view, Light &out);
};
