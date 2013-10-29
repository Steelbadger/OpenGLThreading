#pragma once
///////////////////////// By Ross Davies /////////////////////////
//	The shader class represents a single shader source file		//
//	handles parsing and loading of the source file				//
//////////////////////////////////////////////////////////////////
//	The shaderprogram class represents a complete shader		//
//	program, this is the primary object used for manipulating	//
//	the shaders and material functionality of an object			//
//////////////////////////////////////////////////////////////////

#include "glew.h"
#include "texture.h"
#include "uniformlocations.h"

#include <string>
#include <map>
#include <vector>

class Shader
{
public:
	Shader(void);
	//  Create a shader component from file (by string)
	Shader(std::string);
	void Create(std::string);
	//  Get the OpenGL shader component reference
	GLuint Reference(){return shaderRef;}
	~Shader(void);
private:
	GLuint shaderRef;

	//  Storage for already loaded shader components to reduce unnecessary initialization
	static std::map<std::string, GLuint> ShaderLibrary;
};

class ShaderProgram
{
public:
	ShaderProgram();
	~ShaderProgram();

	//  Add shader components to the program
	void AddShader(Shader s);

	//  Compile and link the shader
	void Compile();

	//  The OpenGl shader program reference
	GLuint Reference(){return programRef;}

	//  Return the uniform locations (shader input variables) for the program
	UniformLocations GetUniforms(){return uniforms;}	
private:
	GLuint programRef;
	std::vector<Shader> shaders;
	UniformLocations uniforms;

	void FindUniformLocations();
};

