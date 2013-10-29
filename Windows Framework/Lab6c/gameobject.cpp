#include "gameobject.h"
#include "myvector4.h"
#include "cameramodule.h"
#include "rigidbodymodule.h"
#include "mesh.h"
#include "controller.h"
#include "quaternion.h"

//  Define vectors for the global axes
const Vector4 GameObject::GLOBALX(1.0f, 0.0f, 0.0f, 1.0f);
const Vector4 GameObject::GLOBALY(0.0f, 1.0f, 0.0f, 1.0f);
const Vector4 GameObject::GLOBALZ(0.0f, 0.0f, 1.0f, 1.0f);


GameObject::GameObject(void):
	position(0.0f, 0.0f, 0.0f, 1.0f),
		rotation(),
		localX(GLOBALX),
		localY(GLOBALY),
		localZ(GLOBALZ),
		scale(1.0f, 1.0f, 1.0f),
		parent(NULL)
{
}


GameObject::~GameObject(void)
{
}

void GameObject::SetLocation(float xNew, float yNew, float zNew)
/*-------------------------------------------------------------------------*\
|	Purpose:	Move the object to position at (xNew, yNew, zNew)			|
|																			|
|	Parameters:	The x, y, z position to move to							 	|
|																			|
\*-------------------------------------------------------------------------*/
{
	position.x = xNew;
	position.y = yNew;
	position.z = zNew;
	position.w = 1.0f;
}

void GameObject::SetRotate(float xRotNew, float yRotNew, float zRotNew)
/*-------------------------------------------------------------------------*\
|	Purpose:	Set the orientation to specific values around the x, y 		|
|				and z axes.													|
|																			|
|	Parameters:	The desired rotations around the x, y and z axes		 	|
|																			|
\*-------------------------------------------------------------------------*/
{
	Quaternion xRotation;
	Quaternion yRotation;
	Quaternion zRotation;

	//  Build a quaternion for each rotation component (around each axis)
	if (xRotNew == 0.0f) {
		xRotation = Quaternion(1.0f, 0.0f, 0.0f, 0.0f);
	} else {
		xRotation.Rotation(xRotNew, 1.0f, 0.0f, 0.0f);
	}

	if (yRotNew == 0.0f) {
		yRotation = Quaternion(1.0f, 0.0f, 0.0f, 0.0f);
	} else {
		yRotation.Rotation(yRotNew, 0.0f, 1.0f, 0.0f);
	}

	if (zRotNew == 0.0f) {
		zRotation = Quaternion(1.0f, 0.0f, 0.0f, 0.0f);
	} else {
		zRotation.Rotation(zRotNew, 0.0f, 0.0f, 1.0f);
	}

	//  Combine them to get the resultant rotation
	rotation = xRotation * yRotation * zRotation;
	rotation.NormalizeSelf();
}

void GameObject::SetRotate(Quaternion q)
/*-------------------------------------------------------------------------*\
|	Purpose:	Set the orientation to a known Quaternion					|
|																			|
|	Parameters:	The desired orientation as a Quaternion					 	|
|																			|
\*-------------------------------------------------------------------------*/
{
	rotation = q;
	rotation.NormalizeSelf();
}

void GameObject::MoveDeltaX(float dx)
/*-------------------------------------------------------------------------*\
|	Purpose:	Move object along global X axis								|
|																			|
|	Parameters:	Distance along axis to move								 	|
|																			|
\*-------------------------------------------------------------------------*/
{
	position.x += dx;
}
void GameObject::MoveDeltaY(float dy)
/*-------------------------------------------------------------------------*\
|	Purpose:	Move object along global Y axis								|
|																			|
|	Parameters:	Distance along axis to move								 	|
|																			|
\*-------------------------------------------------------------------------*/
{
	position.y += dy;
}
void GameObject::MoveDeltaZ(float dz)
/*-------------------------------------------------------------------------*\
|	Purpose:	Move object along global Z axis								|
|																			|
|	Parameters:	Distance along axis to move								 	|
|																			|
\*-------------------------------------------------------------------------*/
{
	position.z += dz;
}

void GameObject::MoveLocalDeltaX(float dx)
/*-------------------------------------------------------------------------*\
|	Purpose:	Move object along local X axis								|
|																			|
|	Parameters:	Distance along axis to move								 	|
|																			|
\*-------------------------------------------------------------------------*/
{
	//  Find the local axis by applying the orientation to the global axis
	localX = rotation * Quaternion(GLOBALX) * rotation.Inverse();
	//  Create translation vector
	Vector4 translator = localX.Normalise();
	translator *= dx;

	//  Create translation matrix
	Matrix4x4 TranslationMatrix;
	TranslationMatrix.Translation(translator.x, translator.y, translator.z);

	//  Translate our position
	position = TranslationMatrix * position;
}
void GameObject::MoveLocalDeltaY(float dy)
/*-------------------------------------------------------------------------*\
|	Purpose:	Move object along local Y axis								|
|																			|
|	Parameters:	Distance along axis to move								 	|
|																			|
\*-------------------------------------------------------------------------*/
{
	//  Find the local axis by applying the orientation to the global axis
	localY = rotation * Quaternion(GLOBALY) * rotation.Inverse();
	//  Create translation vector
	Vector4 translator = localY.Normalise();
	translator *= dy;
	//  Create translation matrix
	Matrix4x4 TranslationMatrix;
	TranslationMatrix.Translation(translator.x, translator.y, translator.z);
	//  Translate our position
	position = TranslationMatrix * position;
}
void GameObject::MoveLocalDeltaZ(float dz)
/*-------------------------------------------------------------------------*\
|	Purpose:	Move object along local Z axis								|
|																			|
|	Parameters:	Distance along axis to move								 	|
|																			|
\*-------------------------------------------------------------------------*/
{
	//  Find the local axis by applying the orientation to the global axis
	localZ = rotation * Quaternion(GLOBALZ) * rotation.Inverse();
	//  Create translation vector
	Vector4 translator = localZ.Normalise();
	translator *= dz;
	//  Create translation matrix
	Matrix4x4 TranslationMatrix;
	TranslationMatrix.Translation(translator.x, translator.y, translator.z);
	//  Translate our position
	position = TranslationMatrix * position;
}

void GameObject::RotateDeltaX(float dx)
/*-------------------------------------------------------------------------*\
|	Purpose:	Rotate object by angle around global x axis					|
|																			|
|	Parameters:	Angle through which to move								 	|
|																			|
\*-------------------------------------------------------------------------*/
{
	//  Create a quaternion representing a rotation of dx degrees around the global X axis
	Quaternion rot;
	rot.Rotation(dx, GLOBALX);

	//  Combine the new rotation with the existing rotation to get result
	rotation = rot * rotation;
	rotation.NormalizeSelf();

	//  Recalculate the local axes with the new orientation
	localX = rotation * Quaternion(GLOBALX) * rotation.Inverse();
	localY = rotation * Quaternion(GLOBALY) * rotation.Inverse();
	localZ = rotation * Quaternion(GLOBALZ) * rotation.Inverse();

	//  Ensure new axes are unit vectors
	localX.NormaliseSelf();
	localY.NormaliseSelf();
	localZ.NormaliseSelf();
}
void GameObject::RotateDeltaY(float dy)
/*-------------------------------------------------------------------------*\
|	Purpose:	Rotate object by angle around global y axis					|
|																			|
|	Parameters:	Angle through which to move								 	|
|																			|
\*-------------------------------------------------------------------------*/
{
	//  Create a quaternion representing a rotation of dy degrees around the global Y axis
	Quaternion rot;
	rot.Rotation(dy, GLOBALY);

	//  Combine the new rotation with the existing rotation to get result
	rotation = rot * rotation;
	rotation.NormalizeSelf();

	//  Recalculate the local axes with the new orientation
	localX = rotation * Quaternion(GLOBALX) * rotation.Inverse();
	localY = rotation * Quaternion(GLOBALY) * rotation.Inverse();
	localZ = rotation * Quaternion(GLOBALZ) * rotation.Inverse();

	//  Ensure new axes are unit vectors
	localX.NormaliseSelf();
	localY.NormaliseSelf();
	localZ.NormaliseSelf();

}
void GameObject::RotateDeltaZ(float dz)
/*-------------------------------------------------------------------------*\
|	Purpose:	Rotate object by angle around global z axis					|
|																			|
|	Parameters:	Angle through which to move								 	|
|																			|
\*-------------------------------------------------------------------------*/
{
	//  Create a quaternion representing a rotation of dz degrees around the global Z axis
	Quaternion rot;
	rot.Rotation(dz, GLOBALZ);

	//  Combine the new rotation with the existing rotation to get result
	rotation = rot * rotation;
	rotation.NormalizeSelf();

	//  Recalculate the local axes with the new orientation
	localX = rotation * Quaternion(GLOBALX) * rotation.Inverse();
	localY = rotation * Quaternion(GLOBALY) * rotation.Inverse();
	localZ = rotation * Quaternion(GLOBALZ) * rotation.Inverse();

	//  Ensure new axes are unit vectors
	localX.NormaliseSelf();
	localY.NormaliseSelf();
	localZ.NormaliseSelf();
}

void GameObject::RotateLocalDeltaX(float dx)
/*-------------------------------------------------------------------------*\
|	Purpose:	Rotate object by angle around local x axis					|
|																			|
|	Parameters:	Angle through which to move								 	|
|																			|
\*-------------------------------------------------------------------------*/
{
	//  Find the rotation axis by rotating global X by the current orientation quaternion
	Vector4 rotationAxis = rotation * Quaternion(GLOBALX) * rotation.Inverse();

	//  Create a quaternion representing a rotation of dx degrees around the local x axis
	Quaternion rot;
	rot.Rotation(dx, rotationAxis);

	//  Combine the new rotation with the existing rotation to get result
	rotation = rot * rotation;
	rotation.NormalizeSelf();

	//  Recalculate the local axes with the new orientation
	localX = rotation * Quaternion(GLOBALX) * rotation.Inverse();
	localY = rotation * Quaternion(GLOBALY) * rotation.Inverse();
	localZ = rotation * Quaternion(GLOBALZ) * rotation.Inverse();

	//  Ensure new axes are unit vectors
	localX.NormaliseSelf();
	localY.NormaliseSelf();
	localZ.NormaliseSelf();
}
void GameObject::RotateLocalDeltaY(float dy)
/*-------------------------------------------------------------------------*\
|	Purpose:	Rotate object by angle around local y axis					|
|																			|
|	Parameters:	Angle through which to move								 	|
|																			|
\*-------------------------------------------------------------------------*/
{
	//  Find the rotation axis by rotating global Y by the current orientation quaternion
	Vector4 rotationAxis = rotation * Quaternion(GLOBALY) * rotation.Inverse();

	//  Create a quaternion representing a rotation of dy degrees around the local y axis
	Quaternion rot;
	rot.Rotation(dy, rotationAxis);

	//  Combine the new rotation with the existing rotation to get result
	rotation = rot * rotation;
	rotation.NormalizeSelf();

	//  Recalculate the local axes with the new orientation
	localX = rotation * Quaternion(GLOBALX) * rotation.Inverse();
	localY = rotation * Quaternion(GLOBALY) * rotation.Inverse();
	localZ = rotation * Quaternion(GLOBALZ) * rotation.Inverse();

	//  Ensure new axes are unit vectors
	localX.NormaliseSelf();
	localY.NormaliseSelf();
	localZ.NormaliseSelf();
}
void GameObject::RotateLocalDeltaZ(float dz)
/*-------------------------------------------------------------------------*\
|	Purpose:	Rotate object by angle around local z axis					|
|																			|
|	Parameters:	Angle through which to move								 	|
|																			|
\*-------------------------------------------------------------------------*/
{
	//  Find the rotation axis by rotating global Y by the current orientation quaternion
	Vector4 rotationAxis = rotation * Quaternion(GLOBALZ) * rotation.Inverse();

	//  Create a quaternion representing a rotation of dz degrees around the local z axis
	Quaternion rot;
	rot.Rotation(dz, rotationAxis);

	//  Combine the new rotation with the existing rotation to get result
	rotation = rot * rotation;
	rotation.NormalizeSelf();

	//  Recalculate the local axes with the new orientation
	localX = rotation * Quaternion(GLOBALX) * rotation.Inverse();
	localY = rotation * Quaternion(GLOBALY) * rotation.Inverse();
	localZ = rotation * Quaternion(GLOBALZ) * rotation.Inverse();

	//  Ensure new axes are unit vectors
	localX.NormaliseSelf();
	localY.NormaliseSelf();
	localZ.NormaliseSelf();
}

Vector4 GameObject::GetPosition()
/*-------------------------------------------------------------------------*\
|	Purpose:	Retrieve the position of the object							|
|																			|
|	Returns:	Object position											 	|
|																			|
\*-------------------------------------------------------------------------*/
{
	//  GameObjects can have parent objects, if they do then the position
	//  of the child GameObject is an offset from the parent (hierarchical modelling)
	if (parent == NULL) {
		return position;
	} else {
		return position + parent->GetPosition();
	}
}

Vector4 GameObject::GetLocalX()
/*-------------------------------------------------------------------------*\
|	Purpose:	Retrieve the direction of the local X axis					|
|																			|
|	Returns:	The local X axis										 	|
|																			|
\*-------------------------------------------------------------------------*/
{
	//  GameObjects can have parent objects, if they do then the orientation
	//  of the child GameObject is an offset from the parent (hierarchical modelling)
	if (parent == NULL) {
		return localX;
	} else {
		return parent->GetRotation() * Quaternion(localX) * parent->GetRotation().Inverse();
	}
}

Vector4 GameObject::GetLocalY()
/*-------------------------------------------------------------------------*\
|	Purpose:	Retrieve the direction of the local Y axis					|
|																			|
|	Returns:	The local Y axis										 	|
|																			|
\*-------------------------------------------------------------------------*/
{
	//  GameObjects can have parent objects, if they do then the orientation
	//  of the child GameObject is an offset from the parent (hierarchical modelling)
	if (parent == NULL) {
		return localY;
	} else {
		return parent->GetRotation() * Quaternion(localY) * parent->GetRotation().Inverse();
	}
}

Vector4 GameObject::GetLocalZ()
/*-------------------------------------------------------------------------*\
|	Purpose:	Retrieve the direction of the local Z axis					|
|																			|
|	Returns:	The local Z axis										 	|
|																			|
\*-------------------------------------------------------------------------*/
{
	//  GameObjects can have parent objects, if they do then the orientation
	//  of the child GameObject is an offset from the parent (hierarchical modelling)
	if (parent == NULL) {
		return localZ;
	} else {
		return parent->GetRotation() * Quaternion(localZ) * parent->GetRotation().Inverse();
	}
}

Quaternion GameObject::GetRotation()
/*-------------------------------------------------------------------------*\
|	Purpose:	Retrieve the orientation quaternion of the object			|
|																			|
|	Returns:	The orientiation as a quaternion						 	|
|																			|
\*-------------------------------------------------------------------------*/
{
	//  GameObjects can have parent objects, if they do then the orientation
	//  of the child GameObject is an offset from the parent (hierarchical modelling)
	if (parent == NULL) {
		return rotation;
	} else {
		return parent->GetRotation() * rotation;
	}
}

CameraModule* GameObject::GetCamera()
/*-------------------------------------------------------------------------*\
|	Purpose:	Empty function handle for inheriting objects to use			|
|																			|
|	Returns:	A camera attached to the object, none in this case		 	|
|																			|
\*-------------------------------------------------------------------------*/
{
	return NULL;
}

Rigidbody* GameObject::GetRigidbody()
/*-------------------------------------------------------------------------*\
|	Purpose:	Empty function handle for inheriting objects to use			|
|																			|
|	Returns:	A rigidbody attached to the object, none in this case	 	|
|																			|
\*-------------------------------------------------------------------------*/
{
	return NULL;
}

Mesh* GameObject::GetMesh()
/*-------------------------------------------------------------------------*\
|	Purpose:	Empty function handle for inheriting objects to use			|
|																			|
|	Returns:	A mesh attached to the object, none in this case		 	|
|																			|
\*-------------------------------------------------------------------------*/
{
	return NULL;
}

Controller* GameObject::GetController()
/*-------------------------------------------------------------------------*\
|	Purpose:	Empty function handle for inheriting objects to use			|
|																			|
|	Returns:	A controller attached to the object, none in this case	 	|
|																			|
\*-------------------------------------------------------------------------*/
{
	return NULL;
}

GameObject* GameObject::GetParent()
/*-------------------------------------------------------------------------*\
|	Purpose:	Used for hierarchical modelling, each GameObject can be		|
|				a child of another, this function gets that parent			|
|																			|
|	Returns:	A pointer to the GameObject to which this GameObject is		|
|				attached													|
|																			|
\*-------------------------------------------------------------------------*/
{
	return parent;
}

void GameObject::SetParent(GameObject &g)
/*-------------------------------------------------------------------------*\
|	Purpose:	Used for hierarchical modelling, each GameObject can be		|
|				a child of another, this function sets that parent			|
|																			|
|	Parameters:	A pointer to the GameObject to which this GameObject should	|
|				be attached													|
|																			|
\*-------------------------------------------------------------------------*/
{
	parent = &g;
}

void GameObject::UniformScale(float scaleFactor)
/*-------------------------------------------------------------------------*\
|	Purpose:	Scale object uniformly along all axes						|
|																			|
|	Parameters:	Factor by which to scale (function will compound scaling	|
|				operations)													|
|																			|
\*-------------------------------------------------------------------------*/
{
	scale = Vector4(scale) * scaleFactor;
}

void GameObject::SetScale(float scaleFactor)
/*-------------------------------------------------------------------------*\
|	Purpose:	Set the scale of an object to known value					|
|																			|
|	Parameters:	Factor to which to scale									|
|																			|
\*-------------------------------------------------------------------------*/
{
	scale.x = scaleFactor;
	scale.y = scaleFactor;
	scale.z = scaleFactor;
}

void GameObject::NonUniformScale(float xScale, float yScale, float zScale)
/*-------------------------------------------------------------------------*\
|	Purpose:	Scale object non uniformly along each axis					|
|																			|
|	Parameters:	Factors by which to scale in each axis (function will		|
|				compound scaling operations)								|
|																			|
\*-------------------------------------------------------------------------*/
{
	scale.x *= xScale;
	scale.y *= yScale;
	scale.z *= zScale;
}

