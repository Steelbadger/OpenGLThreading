#pragma once
///////////////////////// By Ross Davies /////////////////////////
//				Mouse Input manager class, SINGLETON			//
//  This class monitors the windows messages for input from		//
//							mouse.								//
//////////////////////////////////////////////////////////////////

#include <windows.h>

class Mouse
{
public:

	enum axis {X, Y};

	Mouse(void);
	~Mouse(void);

	//  Get the position of the mouse (pixels)
	int GetX() {return x;}
	int GetY() {return y;}
	int Location(axis XorY);

	//  Handle windows messages
	void Message(UINT, WPARAM, LPARAM);

	//  Update pressed/released states
	void Update();

	//  Get button states and pressed/released states
	bool RButton() {return rmouse;}
	bool LButton() {return lmouse;}
	bool MButton() {return mmouse;}
	bool OldRButton() {return lastrmouse;}
	bool OldLButton() {return lastlmouse;}
	bool OldMButton() {return lastmmouse;}
	bool rClick() {return rmouseclick;}
	bool lClick() {return lmouseclick;}
	bool mClick() {return mmouseclick;}
	bool lRelease() {return lrelease;}
	bool rRelease() {return rrelease;}
	bool mRelease() {return mrelease;}

	//  Get the distance the mouse has moved from the position to which it was locked
	int FrameDX(){return (x-Lockedx);}
	int FrameDY(){return (y-Lockedy);}

	//  Tell the mouse that it has been locked to a position
	void SetLocked(int xl, int yl){Lockedx = xl;Lockedy=yl;x=xl;y=yl;}

	//  Get the distance the mouse wheel has moved
	int GetWheelDelta(){return wheelDelta;}

	//  Check if the mouse wheel has moved
	bool CheckWheelMoved(){return wheelWasMoved;}

private:
	//  Position
	int x, y;
	//  Position last frame
	int oldx, oldy;
	//  Position to which the mouse is locked
	int Lockedx, Lockedy;

	//  Button state variables
	bool lmouse, rmouse, mmouse;
	bool lastlmouse, lastrmouse, lastmmouse;
	bool lmouseclick, rmouseclick, mmouseclick;
	bool lrelease, rrelease, mrelease;

	//  Wheel state variables
	bool wheelMoved;
	bool wheelWasMoved;

	//  Distance wheel has moved this frame
	int wheelDelta;

	//  Set the x/y position mased on windows message
	void UpdatePosition(LPARAM);
};