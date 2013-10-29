#pragma once
///////////////////////// By Ross Davies /////////////////////////
//	The texture class represents a single texture, this class	//
//	handles loading textures from files or arrays into the		//
//  OpenGL context												//
//////////////////////////////////////////////////////////////////


#include "glew.h"
#include "tex.h"

#include <string>
#include <map>


class Texture
{
public:
	//  Textures must be one of 4 types
	enum Type {DIFFUSE, BUMP, NORMAL, DISPLACEMENT};

	//  Constructors, texture can be loaded from string(file) or an array
	Texture(void);
	Texture(Type t, std::string path);
	Texture(Type t, unsigned short* base, unsigned int length);

	~Texture(void);

	//  Set the image associated with the texture object, can be loaded from
	//  a string (file) or an array.  Special case for handling a displacement
	//  map as this requires a magnitude for the displacements
	void SetTexture(Type t, std::string path);
	void SetTexture(Type t, unsigned short* base, unsigned int length);
	void SetDisplacementMap(unsigned short* base, unsigned int length, float magnitude);

	//  Sends the texture uniform to the shader
	void Apply(int texUnit, GLuint target);

	Type GetType(){return type;}

	float GetDisplacementMagnitude(){return (type == DISPLACEMENT? magnitude : 0.0f);}

	//  Find the OpenGL texture reference
	GLuint Reference(){return texRef;}
private:
	Type type;

	//  The file path
	std::string path;

	//  The openGL reference
	GLuint texRef;

	//  The magnitude (if texture is a displacement map)
	float magnitude;

	//  Has the file been added to the OpenGL context?
	bool created;

	//  The location of the image in memory (as an array)
	unsigned short* imgBase;
	//  and the size of the image (linear, in unsigned shorts)
	int imgSize;

	//  The unique ID of the texture
	int uid;

	//  The OpenGL wrapping mode for the texture
	GLenum wrapping;

	//  A lookup map to reduce unecessary repeat loading of files
	static std::map<int, GLuint> TextureLibrary;
	static int UNIQUE_ID_COUNTER;
};

