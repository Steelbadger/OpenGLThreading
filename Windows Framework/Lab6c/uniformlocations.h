#pragma once
///////////////////////// By Ross Davies /////////////////////////
//  This struct is used to hold the uniform addresses for		//
//	each shader program, this is generalised for simplicity		//
//////////////////////////////////////////////////////////////////
#include "glew.h"
#include "defines.h"


struct UniformLocations
{
	GLuint ProjectionMatrix;
	GLuint ViewMatrix;
	GLuint ModelMatrix;
	GLuint ModelViewMatrix;
	GLuint NormalMatrix;
	GLuint LightPositions[MAXLIGHTS];
	GLuint LightColours[MAXLIGHTS];
	GLuint NumLights;
	GLuint LightTypes[MAXLIGHTS];
	GLuint Textures[MAX_TEXTURES];
	GLuint MapWidth;
	GLuint Magnitude;
	GLuint Time;
	GLuint XMulFactor;
	GLuint YMulFactor;
};
