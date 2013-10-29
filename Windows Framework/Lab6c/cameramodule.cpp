#include "cameramodule.h"
#include "gameobject.h"


CameraModule::CameraModule(GameObject* parent):
	parent(parent),
	fieldOfView(45.0f),
	nearClipPlane(0.1f),
	farClipPlane(500.0f)
{
}


CameraModule::~CameraModule(void)
{
}

void CameraModule::Zoom(int zoom)
/*-------------------------------------------------------------------------*\
|	Purpose:	Zoom in/out by value (arbitrary proportion)					|
|																			|
|	Parameters:	Amount to zoom in/out by (positive out, negative in)		|
\*-------------------------------------------------------------------------*/
{
	fieldOfView -= zoom/120;
	if (fieldOfView > 45.0f) {
		fieldOfView = 45.0f;
	} else if (fieldOfView < 1.0f) {
		fieldOfView = 1.0f;
	}
}

void CameraModule::SetWindowSize(int width, int height)
/*-------------------------------------------------------------------------*\
|	Purpose:	Save the size of the target window							|
|																			|
|	Parameters:	Width and height of context window							|
\*-------------------------------------------------------------------------*/
{
	windowWidth = width;
	windowHeight = height;
}

void CameraModule::SetWindowSize()
/*-------------------------------------------------------------------------*\
|	Purpose:	Save the size of the target window, using pointer to		|
|				that window													|
\*-------------------------------------------------------------------------*/
{
	if (targetWindow != NULL) {
		windowWidth = targetWindow->GetWidth();
		windowHeight = targetWindow->GetHeight();
	}
}

void CameraModule::SetClipPlanes(float n, float f)
/*-------------------------------------------------------------------------*\
|	Purpose:	Set the near and far clip planes							|
|																			|
|	Parameters:	Near and far clip plane distances							|
\*-------------------------------------------------------------------------*/
{
	nearClipPlane = n;
	farClipPlane = f;
}

void CameraModule::SetFieldOfView(float fov)
/*-------------------------------------------------------------------------*\
|	Purpose:	Set the frustum angle (field of view)						|
|																			|
|	Parameters:	Field of View												|
\*-------------------------------------------------------------------------*/
{
	fieldOfView = fov;
}

void CameraModule::SetTargetWindow(HWND hWnd)
/*-------------------------------------------------------------------------*\
|	Purpose:	Tell the camera the window to which it's view is being		|
|				rendered													|
|																			|
|	Parameters:	Handle to the window										|
\*-------------------------------------------------------------------------*/
{
	targetWindow = WindowWizard::GetWindowReference(hWnd);
	SetWindowSize();
}

void CameraModule::SetTargetWindow(WindowWizard* window)
/*-------------------------------------------------------------------------*\
|	Purpose:	Tell the camera the window to which it's view is being		|
|				rendered													|
|																			|
|	Parameters:	Pointer to the window object								|
\*-------------------------------------------------------------------------*/
{
	targetWindow = window;
	SetWindowSize();
}

void CameraModule::LockCursorToCentre()
/*-------------------------------------------------------------------------*\
|	Purpose:	Move the onscreen cursor to the centre of the window		|
\*-------------------------------------------------------------------------*/
{
	if (targetWindow != NULL) {
		targetWindow->SetCursorToCentre();
	}
}

Vector4 CameraModule::GetForward()
/*-------------------------------------------------------------------------*\
|	Purpose:	Find the forward vector for the camera						|
|																			|
|	Returns:	Vector4 unit vector forward vector							|
\*-------------------------------------------------------------------------*/
{
	return parent->GetLocalZ();
}

Vector4 CameraModule::GetUp()
/*-------------------------------------------------------------------------*\
|	Purpose:	Find the up vector for the camera							|
|																			|
|	Returns:	Vector4 unit vector up vector								|
\*-------------------------------------------------------------------------*/
{
	Vector4 up = (GetForward().Cross(GameObject::GLOBALY)).Cross(GetForward());

	return up;
}

Vector4 CameraModule::GetPosition()
/*-------------------------------------------------------------------------*\
|	Purpose:	Find the position vector for the camera						|
|																			|
|	Returns:	Vector4 position											|
\*-------------------------------------------------------------------------*/
{
	return parent->GetPosition();
}