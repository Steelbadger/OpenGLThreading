#include "lights.h"
#include "myvector4.h"

LightSource::LightSource(Type t):
	type(t),
	ambient(0.0f),
	colour(0.0f, 0.0f, 0.0f),
	angle(0.0f)
{
}

LightSource::~LightSource(void)
{
}

Light LightSource::GetLightAsStruct(Matrix4x4 view)
/*-------------------------------------------------------------------------*\
|	Purpose:	Return the Light as a minimalistic struct for passing to	|
|				shaders														|
|																			|
|	Parameter:	The view transformation matrix								|
|																			|
|	Returns:	Data struct with all necessary light data for shaders		|
\*-------------------------------------------------------------------------*/
{
	Light out;

	switch (type) {
		case POINT:
			CalculatePoint(view, out);
			break;
		case DIRECTIONAL:
			CalculateDirectional(view, out);
			break;
		case SPOT:
			out.position[0] = GetPosition().x;
			out.position[1] = GetPosition().y;
			out.position[2] = GetPosition().z;
			out.position[3] = 1.0f;
			out.colour[3] = angle;
			out.type = 2;
			break;
	}

	out.colour[0] = colour.x;
	out.colour[1] = colour.y;
	out.colour[2] = colour.z;

	return out;
}

void LightSource::SetAmbient(float b)
/*-------------------------------------------------------------------------*\
|	Purpose:	Set the level of ambient light provided by this light		|
|				shaders														|
|																			|
|	Parameter:	Proportion of the light which is ambient					|
\*-------------------------------------------------------------------------*/
{
	ambient = b;
}

void LightSource::SetColour(float r, float g, float b)
/*-------------------------------------------------------------------------*\
|	Purpose:	Set the colour of the light									|
|																			|
|	Parameter:	The r, g, and b components									|
\*-------------------------------------------------------------------------*/
{
	colour = Vector3(r, g, b);
}

void LightSource::CalculateDirectional(Matrix4x4 view, Light &out)
/*-------------------------------------------------------------------------*\
|	Purpose:	Calculate the data light struct for a directional light		|
|																			|
|	Parameter:	The view matrix, and struct to write to						|
\*-------------------------------------------------------------------------*/
{
	Matrix4x4 transform(Matrix4x4::IDENTITY);
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			transform.elem[i][j] = view.elem[i][j];
		}
	}
	Vector4 pos = GetLocalZ();
	float mul = pos.y + 0.1;
	mul = max(mul, 0.0);
	mul = min(ambient, mul);

	pos = transform * pos;

	out.position[0] = pos.x;
	out.position[1] = pos.y;
	out.position[2] = pos.z;
	out.position[3] = 0.0f;
	out.colour[3] = mul;
	out.type = 1;
}

void LightSource::CalculatePoint(Matrix4x4 view, Light& out)
/*-------------------------------------------------------------------------*\
|	Purpose:	Calculayte the data light struct for a point light			|
|																			|
|	Parameter:	The view matrix, and struct to write to						|
\*-------------------------------------------------------------------------*/
{
	Vector4 pos = GetPosition();

	pos = view * pos;
	out.position[0] = pos.x;
	out.position[1] = pos.y;
	out.position[2] = pos.z;
	out.position[3] = 1.0f;
	out.colour[3] = ambient;
	out.type = 0;
}