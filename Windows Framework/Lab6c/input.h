#pragma once
///////////////////////// By Ross Davies /////////////////////////
//					Input manager class, SINGLETON				//
//  This class monitors the windows messages for input from		//
//  keyboard and mouse.  This class also tracks time between	//
//  frames and tracks updates to the window, though it does		//
//  not store the actual window changes, it just notifies of	//
//							 any change							//
//////////////////////////////////////////////////////////////////
#include "mouse.h"
#include "singleton.h"

#include <windows.h>
#include <time.h>

#define input Input::GetSingleton()



class Input : public CSingleton<Input>
{
public:
	Input(void);
	~Input(void);

	//  Manage windows messages
	void Message(UINT, WPARAM, LPARAM);

	//  Update timers and pressed/released booleans
	void Update();

	//  Getters for all buttons and keys
	int GetMouseX() {return mouse.GetX();}
	int GetMouseY() {return mouse.GetY();}
	bool GetMouseR() {return mouse.RButton();}
	bool GetMouseL() {return mouse.LButton();}
	bool GetMouseM() {return mouse.MButton();}
	bool GetOldMouseR() {return mouse.OldRButton();}
	bool GetOldMouseL() {return mouse.OldLButton();}
	bool GetOldMouseM() {return mouse.OldMButton();}
	bool ReportKeyState(int key) {return keys[key];}
	bool ReportKeyState(char key) {return keys[key];}
	bool ReportKeyPress(int key) {return pressed[key];}
	bool ReportKeyPress(char key) {return pressed[key];}
	bool ReportKeyRelease(int key){return released[key];}
	bool ReportKeyRelease(char key){return released[key];}
	bool ReportLMousePress() {return mouse.lClick();}
	bool ReportRMousePress() {return mouse.rClick();}
	bool ReportMMousePress() {return mouse.mClick();}
	bool ReportLMouseRelease() {return mouse.lRelease();}
	bool ReportRMouseRelease() {return mouse.rRelease();}
	bool ReportMMouseRelease() {return mouse.mRelease();}

	//  Has the window been resized or moved in this frame?
	bool WindowResized() {return windowResized;}
	bool WindowMoved() {return windowMoved;}

	//  Position of the mouse cursor in window coordinates
	int GetMouseDX(){return mouse.FrameDX();}
	int GetMouseDY(){return mouse.FrameDY();}
	int ReportMouseLocation(Mouse::axis Axis) {return mouse.Location(Axis);}

	//  Tell the mouse that it's position has been locked
	void SetMouseLocked(int x, int y){mouse.SetLocked(x, y);}

	//  Check for movement of mousewheel (true/false)
	bool CheckMouseWheel(){return mouse.CheckWheelMoved();}
	//  Get the magnitude of the mouse wheel movement
	int GetMouseWheelDelta(){return mouse.GetWheelDelta();}

	//  Access the timer function of the class
	double GetTimeForLastFrame() {return timeForLastFrame/CLOCKS_PER_SEC;}
	double GetTimeSinceLastFrame();
private:
	//  current state of all keys
	bool keys[256];
	//  state of all keys last frame
	bool oldkey[256];

	//  Has a key been pressed this frame
	bool pressed[256];
	//  Has a key been released this frame
	bool released[256];

	//  Timer variables
	double timeForLastFrame;
	double lastTime;
	double currentTime;

	//  Has the window been modified in any way
	bool windowResized;
	bool windowMoved;

	//  The mouse tracks it's own input
	Mouse mouse;

	//  Helper functions used in message handler
	void KeyDown(UINT);
	void KeyUp(UINT);

};