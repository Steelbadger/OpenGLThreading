#include "material.h"

#include "shader.h"
#include "texture.h"



std::map<std::string, Material> Material::MaterialLibrary;

Material::Material(void)
{
	shaderProgramRef = shaderProgram.Reference();
	compiled = false;
}

Material::Material(std::string n):
	name(n)
{
	shaderProgramRef = shaderProgram.Reference();
	compiled = false;
}


Material::~Material(void)
{
}


void Material::AddTexture(Texture t)
/*-------------------------------------------------------------------------*\
|	Purpose:	Attach a texture object to the material						|
|																			|
|	Parameters:	The texture to attach										|
\*-------------------------------------------------------------------------*/
{
	textures.push_back(t);
	texRefs.push_back(t.Reference());
	//  Special case when the texture being added is a displacement map
	//  get the magnitude of the map out
	if (t.GetType() == Texture::DISPLACEMENT) {
		displacementMapMagnitude = t.GetDisplacementMagnitude();
	}
}

void Material::ReplaceTexture(Texture oldTex, Texture newTex)
/*-------------------------------------------------------------------------*\
|	Purpose:	Remove a previously added texture and replace with			|
|				the new texture												|
|																			|
|	Parameters:	A copy of the texture to remove and the texture to replace	|
|				it with														|
\*-------------------------------------------------------------------------*/
{
	//  Create a default empty texture
	Texture defTex;
	//  Make sure you're not trying to remove that default empty texture
	if (oldTex.Reference() != defTex.Reference()) {
		std::vector<Texture>::iterator it;
		int i = 0;
		//  iterate through the texture vector to find a copy
		for (it = textures.begin(); it != textures.end(); it++) {
			//  Remove it once found
			if ((*it).Reference() == oldTex.Reference()) {
				*it = newTex;
				texRefs[i] = newTex.Reference();
				return;
			}
			i++;
		}
	}

	//  if we tried to remove a default empty texture just add the new texture to the list
	textures.push_back(newTex);
	texRefs.push_back(newTex.Reference());
}

void Material::ReplaceTexture(Texture::Type t, Texture newTex)
/*-------------------------------------------------------------------------*\
|	Purpose:	Remove a previously added texture and replace with			|
|				the new texture												|
|																			|
|	Parameters:	Type of texture to replace and the texture to replace		|
|				it with														|
\*-------------------------------------------------------------------------*/
{
	std::vector<Texture>::iterator it;
	int i = 0;
	for (it = textures.begin(); it != textures.end(); it++) {
		if ((*it).GetType() == t) {
			*it = newTex;
			texRefs[i] = newTex.Reference();
			break;
		}
		i++;
	}
}

void Material::AddShader(std::string shader)
/*-------------------------------------------------------------------------*\
|	Purpose:	Attach a shader object to the material						|
|																			|
|	Parameters:	The file name of the shader to attach						|
\*-------------------------------------------------------------------------*/
{
	Shader newShader(shader);
	shaders.push_back(newShader);
	shaderProgram.AddShader(newShader);
}

std::vector<Texture> Material::GetTextures()
/*-------------------------------------------------------------------------*\
|	Purpose:	Retrieve the full list of attached textures					|
|																			|
|	Returns:	The full list of attached textures							|
\*-------------------------------------------------------------------------*/
{
	return textures;
}

std::vector<Shader> Material::GetShaders()
/*-------------------------------------------------------------------------*\
|	Purpose:	Retrieve the full list of attached shaders					|
|																			|
|	Returns:	The full list of attached shaders							|
\*-------------------------------------------------------------------------*/
{
	return shaders;
}

void Material::AddToMaterialLibrary(std::string n)
/*-------------------------------------------------------------------------*\
|	Purpose:	Add this material to the static library keyed by name		|
|				string, to reduce initialization cost of duplicate materials|
|																			|
|	Parameters:	The name by which to store the material						|
\*-------------------------------------------------------------------------*/
{
	name = n;
	if (!MaterialLibrary.count(name)) {
		MaterialLibrary[name] = *this;
	}
}
void Material::AddToMaterialLibrary()
/*-------------------------------------------------------------------------*\
|	Purpose:	Add this material to the static library keyed by name		|
|				string, to reduce initialization cost of duplicate materials|
\*-------------------------------------------------------------------------*/
{
	if (!MaterialLibrary.count(name)) {
		MaterialLibrary[name] = *this;
	}
}

Material Material::FetchMaterial(std::string name)
/*-------------------------------------------------------------------------*\
|	Purpose:	Retrieve a Material with the name string from the library	|
\*-------------------------------------------------------------------------*/
{
	if (!MaterialLibrary.count(name)){
		return Material(name);
	} else {
		return MaterialLibrary[name];
	}
}

void Material::Apply()
/*-------------------------------------------------------------------------*\
|	Purpose:	active the shader program and apply the textures			|
\*-------------------------------------------------------------------------*/
{
	glUseProgram(shaderProgramRef);
	int numTextures = texRefs.size();

	for (int i = 0; i < numTextures; i++) {
		textures[i].Apply(i, uniforms.Textures[i]);
	}

}

UniformLocations Material::GetUniforms()
/*-------------------------------------------------------------------------*\
|	Purpose:	Retrieve the shader uniform locations						|
\*-------------------------------------------------------------------------*/
{
	return uniforms;
}

void Material::Compile()
/*-------------------------------------------------------------------------*\
|	Purpose:	Compile and link the shader program for use					|
\*-------------------------------------------------------------------------*/
{
	if(compiled == false) {
		shaderProgram.Compile();
		uniforms = shaderProgram.GetUniforms();
		shaderProgramRef = shaderProgram.Reference();
		compiled = true;
	}
}