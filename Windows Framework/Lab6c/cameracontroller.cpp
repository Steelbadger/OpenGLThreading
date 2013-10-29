#include "cameracontroller.h"
#include "input.h"


CameraController::CameraController(CameraModule* cam):
	camera(cam)
{
}


CameraController::~CameraController(void)
{
}

void CameraController::CheckInputAndUpdate()
/*-------------------------------------------------------------------------*\
|	Purpose:	Check the input singleton instance for input pertinent		|
|				to camera control function (zoom and frustum modification)	|
\*-------------------------------------------------------------------------*/
{
	//  Zoom with mouse wheel
	if (input.CheckMouseWheel()) {
		camera->Zoom(input.GetMouseWheelDelta());
	}
	//  Mouselook lock mouse to middle of window
	if (input.GetMouseR()) {
		camera->LockCursorToCentre();
	}

	//  Change frustum clip depths
	if (input.ReportKeyState(VK_UP)) {
		camera->SetClipPlanes(camera->GetNearClipPlane(), camera->GetFarClipPlane()+10.0f);
	}
	if (input.ReportKeyState(VK_DOWN)) {
		camera->SetClipPlanes(camera->GetNearClipPlane(), camera->GetFarClipPlane()-10.0f);
	}

	//  Tell the camera that the window has changed size
	if (input.WindowResized()) {
		camera->SetWindowSize();
	}
}
