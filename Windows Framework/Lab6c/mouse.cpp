#ifndef _WINDOWS_H_DEFINED_
#define _WINDOWS_H_DEFINED_
#include <windows.h>
#include <WindowsX.h>
#endif
#include "mouse.h"
#include <math.h>
#include <stdio.h>

Mouse::Mouse(void)
{
	x = 0;
	y = 0;
	oldx = 0;
	oldy = 0;
	rmouse = false;
	lmouse = false;
	mmouse = false;
}


Mouse::~Mouse(void)
{
}

void Mouse::UpdatePosition(LPARAM lParam)
/*-------------------------------------------------------------------------*\
|	Purpose:	Helper function for dealing with Mouse move message			|
|				updates the mouse's saved position to the new position		|
|																			|
|	Parameters:	Windows message LPARAM										|
|																			|
\*-------------------------------------------------------------------------*/
{
	//  Remember where we were
	oldx = x;
	oldy = y;

	//  Find where we are now.  Add some magic number madness to account for the border around the window
	x = GET_X_LPARAM(lParam)+8; 
	y = GET_Y_LPARAM(lParam)+30; 
}

void Mouse::Message(UINT message, WPARAM wParam, LPARAM lParam)
/*-------------------------------------------------------------------------*\
|	Purpose:	Handle window messages related to the mouse state			|
|				set the various states to mirror the hardware				|
|																			|
|	Parameters:	Windows message parameters									|
|																			|
\*-------------------------------------------------------------------------*/
{
	switch (message)
	{
		case WM_MOUSEMOVE:
			UpdatePosition(lParam);
			break;
		case WM_MOUSEWHEEL:
			wheelDelta = GET_WHEEL_DELTA_WPARAM(wParam);
			wheelMoved = true;
			break;
		case WM_MBUTTONDOWN:
			mmouse = true;
			break;
		case WM_MBUTTONUP:
			mmouse = false;
			break;
		case WM_LBUTTONDOWN:
			lmouse = true;
			break;	
		case WM_LBUTTONUP:
			lmouse = false;
			break;
		case WM_RBUTTONDOWN:
			rmouse = true;
			break;
		case WM_RBUTTONUP:
			rmouse = false;
			break;
	}
}

void Mouse::Update()
/*-------------------------------------------------------------------------*\
|	Purpose:	Update the input state (mouse buttons states)				|
|																			|
\*-------------------------------------------------------------------------*/
{
	//  Reset our pressed/released values
	lmouseclick = false;
	rmouseclick = false;
	mmouseclick = false;
	lrelease = false;
	rrelease = false;
	mrelease = false;

	//  Remember if the wheel was moved last frame
	wheelWasMoved = wheelMoved;

	//  If the wheel has not moved this frame then set distance to zero
	if (wheelMoved == false) {
		wheelDelta = 0;
	}
	//  Reset the wheel moved state for the new frame
	wheelMoved = false;

	//  Compare state this frame to state last frame to find changes
	if (lmouse == true && lastlmouse == false)
	{
		lmouseclick = true;
	}
	if (rmouse == true && lastrmouse == false)
	{
		rmouseclick = true;
	}
	if (mmouse == true && lastmmouse == false)
	{
		mmouseclick = true;
	}
	if (lmouse == false && lastlmouse == true)
	{
		lrelease = true;
	}
	if (rmouse == false && lastrmouse == true)
	{
		rrelease = true;
	}
	if (mmouse == false && lastmmouse == true)
	{
		mrelease = true;
	}

	//  And remember our state from this frame
	lastlmouse = lmouse;
	lastrmouse = rmouse;
	lastmmouse = mmouse;
}

int Mouse::Location(axis Axis)
/*-------------------------------------------------------------------------*\
|	Purpose:	DEPRECATED, additional function for getting the position	|
|				components of the mouse										|
|																			|
|	Parameters:	The axis for which we wish to know the mouse's position		|
|																			|
|	Returns:	The mouse's position in pixels along requested axis			|
|																			|
\*-------------------------------------------------------------------------*/
{
	if (Axis == X)
		return x;
	else
		return y;
}