#pragma once
///////////////////////// By Ross Davies /////////////////////////
//	The basic GameObject class from which all objects			//
//	inherit, provides positions and rotations (using			//
//	quaternions) for all meshes, colliders and cameras			//
//////////////////////////////////////////////////////////////////
#include "cameramodule.h"
#include "controller.h"
#include "mesh.h"
#include "rigidbodymodule.h"


class GameObject
{
public:
	GameObject(void);
	~GameObject(void);

	void SetLocation(float x, float y, float z);
	void SetRotate(float xRot, float yRot, float zRot);
	void SetRotate(Quaternion rotation);

	//  Move Object along global Axes
	void MoveDeltaX(float dx);
	void MoveDeltaY(float dy);
	void MoveDeltaZ(float dz);

	//  Move object along local axes
	void MoveLocalDeltaX(float dx);
	void MoveLocalDeltaY(float dy);
	void MoveLocalDeltaZ(float dz);

	//  Rotate object around global axes
	void RotateDeltaX(float dx);
	void RotateDeltaY(float dy);
	void RotateDeltaZ(float dz);

	//  rotate object about local axes
	void RotateLocalDeltaX(float dx);
	void RotateLocalDeltaY(float dy);
	void RotateLocalDeltaZ(float dz);

	//  Incremental scaling (scale object with respect to previous scale)
	void UniformScale(float scaleFactor);

	//  Set object scale (scale object with respect to a scale of 1.0f)
	void SetScale(float scaleFactor);

	//  Scale object by different amounts in each axis
	void NonUniformScale(float xScale, float yScale, float zScale);

	//  Getters
	Vector3 GetScale(){return scale;}
	Vector4 GetPosition();
	Vector4 GetLocalX();
	Vector4 GetLocalY();
	Vector4 GetLocalZ();
	Quaternion GetRotation();

	//  Virtual get hooks for inheriting classes
	virtual CameraModule* GetCamera();
	virtual Rigidbody* GetRigidbody();
	virtual Mesh* GetMesh();
	virtual Controller* GetController();
	virtual GameObject* GetParent();

	//  A gameobject can have another gameobject as parent (replacement for matrix stack)
	void SetParent(GameObject& target);

	//  These probably shouldn't be scoped here
	static const Vector4 GLOBALX;
	static const Vector4 GLOBALY;
	static const Vector4 GLOBALZ;
private:
	//  The parent gameobject, all transformations are respective to this gameobject
	GameObject* parent;

	//  Object state
	Vector4 position;
	Quaternion rotation;
	Vector3 scale;
	Vector4 localX;
	Vector4 localY;
	Vector4 localZ;
};



