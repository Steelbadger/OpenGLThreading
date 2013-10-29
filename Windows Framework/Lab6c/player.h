#pragma once
///////////////////////// By Ross Davies /////////////////////////
//	Player class, an extension of the GameObject class with		//
//	explicit containment of controllers or input response		//
//////////////////////////////////////////////////////////////////
#include "cameramodule.h"
#include "cameracontroller.h"
#include "controller.h"
#include "gameobject.h"
#include "mesh.h"
#include "rigidbodymodule.h"
#include "Terrain.h"

class NoiseObject;

class Player : public GameObject
{
public:
	Player(void);
	~Player(void);

	void InputUpdate();
	void SetCameraTargetWindow(WindowWizard* window);
	Controller controller;

	void CheckGroundCollision(NoiseObject n);

	virtual CameraModule* GetCamera();
	virtual Rigidbody* GetRigidbody();
	virtual Mesh* GetMesh();
	virtual Controller* GetController();

private:
	bool flying;

	CameraModule camera;
	CameraController camController;
	Rigidbody rigidbody;

};

