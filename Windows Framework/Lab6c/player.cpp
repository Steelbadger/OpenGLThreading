#include "player.h"
#include "noisegenerator.h"


Player::Player(void):
		controller(this),
		camera(this),
		camController(&camera),
		flying(true),
		rigidbody(this)
{
}


Player::~Player(void)
{
}

void Player::InputUpdate()
//  The main update function of the player object, this calls
//  the update functions of attached controllers and cameras
//  and the rigidbody (rigidbody provides gravity)
{
	controller.CheckInputAndAct();
	camController.CheckInputAndUpdate();

	//  Not part of the controller class
	//  Switch in and out of flying mode (activate or
	//  deactivate the rigidbody)
	if (input.ReportKeyPress('T')) {
		flying = !flying;
		if (flying == false){
			controller.SetSensitivity(8.0f);
			rigidbody.SetActive();
		} else {
			controller.SetSensitivity(50.0f);
			rigidbody.SetInactive();
		}
	}
	rigidbody.Update();
}


void Player::SetCameraTargetWindow(WindowWizard* window)
//  Find the window to which the attached camera is drawing
{
	camera.SetTargetWindow(window);
}

void Player::CheckGroundCollision(NoiseObject n)
//  If the player touches the ground then they're no longer jumping
{
	n.octaves = 9;
	if (rigidbody.CheckGroundCollision(n)) {
		controller.SetJumping(false);
	}
}

CameraModule* Player::GetCamera()
{
	return &camera;
}

Rigidbody* Player::GetRigidbody()
{
	return &rigidbody;
}

Controller* Player::GetController()
{
	return &controller;
}

Mesh* Player::GetMesh()
{
	return NULL;
}
