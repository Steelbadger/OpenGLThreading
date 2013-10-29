#pragma once
///////////////////////// By Ross Davies /////////////////////////
//	Special controller for modifying a camera attached to the	//
//	parent game object											//
//////////////////////////////////////////////////////////////////
#include "cameramodule.h"

class CameraController
{
public:
	CameraController(CameraModule* camera);
	~CameraController(void);

	void CheckInputAndUpdate();

private:
	CameraModule* camera;
};

