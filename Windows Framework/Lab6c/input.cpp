#include "input.h"
#include "mouse.h"
#include <string.h>
#include <windows.h>

//  Set up the singleton instance
Input ourInputSingleton;

Input::Input(void)
{
	for (int i = 255; i >= 0; i--)
	{
		oldkey[i] = false;
		keys[i] = false;
		pressed[i] = false;
		released[i] = false;
	}
	timeForLastFrame = 0;
	lastTime = clock();
	currentTime = clock();
}


Input::~Input(void)
{
}

void Input::Message(UINT message, WPARAM wParam, LPARAM lParam)
/*-------------------------------------------------------------------------*\
|	Purpose:	Handle the windows messages for input state changes			|
|																			|
|	Parameters:	The windows message parameters								|
|																			|
\*-------------------------------------------------------------------------*/
{

	switch (message)
	{
		case WM_SIZE:
			windowResized = true;
			break;
		case WM_MOVE:
			windowMoved = true;
			break;
		case WM_KEYDOWN:
			KeyDown(wParam);
			break;
		case WM_KEYUP:
			KeyUp(wParam);
			break;
		case WM_MOUSEMOVE:
		case WM_MOUSEWHEEL:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
			mouse.Message(message, wParam, lParam);
			break;
	}

}

void Input::KeyDown(UINT wParam)
/*-------------------------------------------------------------------------*\
|	Purpose:	Helper function for dealing with keypress messages			|
|																			|
|	Parameters:	The key pressed												|
|																			|
\*-------------------------------------------------------------------------*/
{
	oldkey[wParam] = keys[wParam];
	keys[wParam]=true;
}

void Input::KeyUp(UINT wParam)
/*-------------------------------------------------------------------------*\
|	Purpose:	Helper function for dealing with keyrelease messages		|
|																			|
|	Parameters:	The key released											|
|																			|
\*-------------------------------------------------------------------------*/
{
	oldkey[wParam] = keys[wParam];
	keys[wParam]=false;	
}

void Input::Update()
/*-------------------------------------------------------------------------*\
|	Purpose:	Update the input state (timer and pressed/released states)	|
|																			|
\*-------------------------------------------------------------------------*/
{
	//  Do the timing
	lastTime = currentTime;
	currentTime = clock();
	timeForLastFrame = currentTime - lastTime;

	//  The window hasn't been resized/moved yet this frame
	windowResized = false;
	windowMoved = false;


	//  Check for newly pressed or released keys
	for (int i = 256; i > 0; i--)
	{
		if (keys[i-1] == true && oldkey[i-1] == false)
			pressed[i-1] = true;
		else
			pressed[i-1] = false;
	}
	for (int i = 256; i > 0; i--)
	{
		if (keys[i-1] == false && oldkey[i-1] == true)
			released[i-1] = true;
		else
			released[i-1] = false;
	}

	//  Copy the current state into the saved (last frame) state
	memcpy(oldkey, keys, sizeof(keys));

	//  Then update the mouse
	mouse.Update();
}

double Input::GetTimeSinceLastFrame()
/*-------------------------------------------------------------------------*\
|	Purpose:	Access the timing functionality of the class, get the time	|
|				between the current frame and the last						|
|																			|
|	Returns:	The time elapsed for the last frame in seconds				|
|																			|
\*-------------------------------------------------------------------------*/
{
	return clock()-currentTime;
}