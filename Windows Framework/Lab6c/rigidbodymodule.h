#pragma once
///////////////////////// By Ross Davies /////////////////////////
//	Rigidbody component, only use currently is checking player	//
//				ground collisions while walking					//		
//////////////////////////////////////////////////////////////////
#include "myvector4.h"

class GameObject;
class Terrain;
class NoiseObject;

class Rigidbody
{
public:
	Rigidbody(GameObject* parent);
	~Rigidbody(void);

	//  Global gravity
	static float GRAVITY;
	static void SetGravity(float g);

	//  Update the rigidbody (accellerate under gravity)
	void Update();

	//  Check for a collision with ground defined by Noise n
	bool CheckGroundCollision(NoiseObject n);
	
	//  Get object velocity
	Vector4 GetVelocity(){return velocity;}
	//  Set the object's velocity
	void SetVelocity(Vector4 v){velocity = v;}

	//  Enable/disable rigidbody collisions
	void SetInactive(){active = false;}
	void SetActive(){active = true;}
	
private:
	GameObject* parent;
	bool active;

	Vector4 velocity;
};

