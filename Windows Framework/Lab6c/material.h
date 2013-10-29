#pragma once
///////////////////////// By Ross Davies /////////////////////////
//	The material class which contains all material properties	//
//	of the mesh with which it is associated; the textures		//
//	and shader programs.										//
//////////////////////////////////////////////////////////////////

#include "glew.h"
#include "shader.h"
#include "uniformlocations.h"

#include <vector>
#include <map>

class Shader;
class Texture;

class Material
{
public:
	Material(void);
	Material(std::string name);
	~Material(void);

	//  Attach a texture object to the material
	void AddTexture(Texture t);
	//  Create and add a shader (from file) to the material
	void AddShader(std::string shader);
	//  Remove oldTex and replace it with newTex
	void ReplaceTexture(Texture oldTex, Texture newTex);
	//  Get the magnitude of the displacement map
	float GetDisplacementMagnitude(){return displacementMapMagnitude;}
	//  Replace the first texture of type t with the new texture
	void ReplaceTexture(Texture::Type t, Texture newTex);
	
	//  Retrieve the list of textures within the material
	std::vector<Texture> GetTextures();
	//  Retrieve the list of shaders used by the material
	std::vector<Shader> GetShaders();

	//  Get the OpenGL reference to the material shader program
	GLuint GetShaderProgram(){return shaderProgramRef;}

	//  Find the uniform locations (shader input data locations)
	UniformLocations GetUniforms();

	//  Apply the material (set shader program, apply materials)
	void Apply();

	//  Compile the shader program
	void Compile();

	//  Save this material to the static library to reduce replications
	void AddToMaterialLibrary();
	void AddToMaterialLibrary(std::string name);

	//  Get a material out of the material library using it's name string
	static Material FetchMaterial(std::string name);

private:

	//  Texture objects
	std::vector<Texture> textures;
	//  OpenGL texture references
	std::vector<GLuint> texRefs;

	//  The OpenGL location references for the variables
	//  expected by the sahder program
	UniformLocations uniforms;

	//  Storage for all shader components
	std::vector<Shader> shaders;
	//  The linked/compiled shader program used by the material
	ShaderProgram shaderProgram;
	//  The opengl reference for the shader program
	GLuint shaderProgramRef;

	//  The name of the material (for lookups)
	std::string name;

	//  If the material includes a displacement map this holds the magnitude
	float displacementMapMagnitude;

	//  Has the shader program been compiled?
	bool compiled;

	//  Storage to cut out unnecessary initialization of repeated materials
	static std::map<std::string, Material> MaterialLibrary;
};

