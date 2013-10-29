#include "rigidbodymodule.h"
#include "gameobject.h"
#include "noisegenerator.h"


float Rigidbody::GRAVITY = 9.81f;

Rigidbody::Rigidbody(GameObject* p):
	parent(p),
	active(false),
	velocity(0.0f, 0.0f, 0.0f, 1.0f)
{
}


Rigidbody::~Rigidbody(void)
{
}

void Rigidbody::SetGravity(float g)
/*-------------------------------------------------------------------------*\
|	Purpose:	Set the global gravity										|
|																			|
|	Parameter:	New gravity value											|
\*-------------------------------------------------------------------------*/
{
	GRAVITY = g;
}

bool Rigidbody::CheckGroundCollision(NoiseObject n)
/*-------------------------------------------------------------------------*\
|	Purpose:	Check if the parent game object is below the terrain		|
|				heightmap as defined by NoiseObject n						|
|																			|
|	Parameter:	The NoiseObject defining the noise function used			|
\*-------------------------------------------------------------------------*/
{
	//  We generate new noise values rather than sample
	NoiseGenerator ground;
	//  Only bother with 12 octaves, more are unnecessary
	n.octaves = 12;

	//  If the rigidbody is currently active
	if(active) {
		//  Sample the perlin noise function at our position
		float h = ground.SIMDPerlin2D(parent->GetPosition().x, parent->GetPosition().z, n)/ground.MaxAmplitude(n) * n.amplitude;
		//  If the object is less than 2m above the terrain then we have a collision
		if (parent->GetPosition().y < 2.0f+h) {
			//  Set the object back above the terrain
			parent->SetLocation(parent->GetPosition().x, 2.0f+h, parent->GetPosition().z);
			//  Zero the object's speed
			velocity = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
			//  Return true to indicate collision
			return true;
		}

		//  Otherwise accellerate downwards
		if (parent->GetPosition().y > 2.0f+h) {
			velocity -= GameObject::GLOBALY*GRAVITY*input.GetTimeForLastFrame();
			//  No collision
			return false;
		}
	}
	//  If the rigidbody is inactive then it's also not causing the object to move
	velocity = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
	return false;
}

void Rigidbody::Update()
/*-------------------------------------------------------------------------*\
|	Purpose:	Move the object under it's velocity							|
\*-------------------------------------------------------------------------*/
{
	parent->MoveDeltaY(velocity.y*input.GetTimeForLastFrame());
}
