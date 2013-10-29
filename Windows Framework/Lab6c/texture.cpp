#include "texture.h"


int Texture::UNIQUE_ID_COUNTER = 0;
std::map<int, GLuint> Texture::TextureLibrary;

Texture::Texture(void) : texRef(0), created(false), uid(UNIQUE_ID_COUNTER++)
{
}

Texture::Texture(Type t, std::string p) : texRef(0), created(false), uid(UNIQUE_ID_COUNTER++)
{
	SetTexture(t, p);
}

Texture::Texture(Type t, unsigned short* base, unsigned int length) : texRef(0), created(false), uid(UNIQUE_ID_COUNTER++)
{
	SetTexture(t, base, length);
}


Texture::~Texture(void)
{
}

void Texture::SetTexture(Type t, std::string p)
/*-------------------------------------------------------------------------*\
|	Purpose:	Create a texture using a file at location p					|
|																			|
|	Parameters:	The type of the texture and the pathname of the texture file|
\*-------------------------------------------------------------------------*/
{
	type = t;
	path = p;
	GLuint tex;
	//  Load the texture in to OpenGL at the Gluint Ref
	CreateGLTexture(p.c_str(), tex);
	texRef = tex;
	//  Default wrapping is repeat
	wrapping = GL_REPEAT;
	//  Magnitude of non-displacement maps is 0
	magnitude = 0;
	//  Remember that this have been uploaded to OpenGL
	created = true;
	//  Add the texture to the library
	TextureLibrary[uid] = texRef;
}

void Texture::SetTexture(Type t, unsigned short* base, unsigned int length)
/*-------------------------------------------------------------------------*\
|	Purpose:	Create a texture using an array in memory					|
|																			|
|	Parameters:	The type of the texture, a pointer to its location in		|
|				memory and the square size of the image						|
\*-------------------------------------------------------------------------*/
{
	type = t;
	path = "POINTER";
	imgBase = base;
	imgSize = length;

	wrapping = GL_CLAMP;
	magnitude = 80;
}

void Texture::SetDisplacementMap(unsigned short* base, unsigned int length, float m)
/*-------------------------------------------------------------------------*\
|	Purpose:	Create a texture that is a displacement map					|
|																			|
|	Parameters:	The type of the texture, a pointer to its location in		|
|				memory and the square size of the image	and the magnitude	|
|				of the displacement											|
\*-------------------------------------------------------------------------*/
{
	SetTexture(Type::DISPLACEMENT, base, length);
	magnitude = m;
}

void Texture::Apply(int targetUnit, GLuint targetLoc)
/*-------------------------------------------------------------------------*\
|	Purpose:	Upload the texture to the target texture unit				|
|																			|
|	Parameters:	The texture unit to load to and the shader sampler to 		|
|				reference it												|
\*-------------------------------------------------------------------------*/
{
	if (texRef == 0) {
		if (TextureLibrary.count(uid) == 0) {
			GLuint TexID;

			glGenTextures(1, &TexID);				// Create The Texture
			glBindTexture(GL_TEXTURE_2D, TexID);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16, imgSize, imgSize, 0, GL_RGBA, GL_UNSIGNED_SHORT, imgBase);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

			delete[] imgBase;

			texRef = TexID;
			created = true;
			TextureLibrary[uid] = texRef;
		} else {
			texRef = TextureLibrary[uid];
		}
	}

	glActiveTexture(targetUnit+GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texRef);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapping);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapping);
	glUniform1i(targetLoc, targetUnit);
}