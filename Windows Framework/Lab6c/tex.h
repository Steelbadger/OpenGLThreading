#pragma once
#include "glew.h"
#include <windows.h>
#include <stdio.h>
#include <gl/gl.h>
#include <gl/glu.h>


//#define uTGAcompare  {0,0,2, 0,0,0,0,0,0,0,0,0};	// Uncompressed TGA Header
//#define cTGAcompare  {0,0,10,0,0,0,0,0,0,0,0,0};	// Compressed TGA Header

typedef struct
{
	GLubyte Header[12];									// TGA File Header
} TGAHeader;


typedef struct
{
	GLubyte		header[6];								// First 6 Useful Bytes From The Header
	GLuint		bytesPerPixel;							// Holds Number Of Bytes Per Pixel Used In The TGA File
	GLuint		imageSize;								// Used To Store The Image Size When Setting Aside Ram
	GLuint		temp;									// Temporary Variable
	GLuint		type;	
	GLuint		Height;									//Height of Image
	GLuint		Width;									//Width ofImage
	GLuint		Bpp;									// Bits Per Pixel
} TGA;


bool LoadUncompressedTGA(const char * filename, FILE * fTGA);
bool LoadCompressedTGA(const char * filename, FILE * fTGA);
bool CreateGLTexture(const char *name, GLuint & TexID  );