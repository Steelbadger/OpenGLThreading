#pragma once
//  Author - Ross Davies
//  This class creates a window and OpenGL render context
//  Additional functions to retrieve details on the window

#include "input.h"

#include "glew.h"
#include <gl/gl.h>
#include <gl/glu.h>

#include <mmsystem.h>
#include <map>
#include <Windows.h>



class WindowWizard
{
public:

	//  Default Constructor (does not create a visible window
	WindowWizard(void);

	//  Constructor for naming window and prescribing size
	WindowWizard(LPSTR WindowName, int width, int height, HINSTANCE hInstance);
	//  Constructor for naming window, setting size, setting fullscreen and window style variables
	WindowWizard(LPSTR WindowName, int width, int height, HINSTANCE hInstance, bool FullScreen, DWORD Style);

	//  Function to actually create and instantiate the visible window
	void WindowCreate(LPSTR strWindowName, int width, int height, DWORD dwStyle, bool bFullScreen, HINSTANCE hInstance);

	~WindowWizard(void);

	//  Setup OpenGL
	void InitializeGraphics(GLdouble fov);

	//  Recheck the window size/position after a move or resize
	void OnResize();
	void OnMove();

	//  Flip the graphics buffers?
	void FlipBuffers();

	//  Clear the window ready for a new OpenGL draw
	void PrepareForDrawing();

	int GetWidth(){return graphicsRect.right;}
	int GetHeight(){return graphicsRect.bottom;}

	//  Find the position of the top left of the window
	int GetX(){return windowRect.left;};
	int GetY(){return windowRect.top;}

	//  Set the on-screen cursor to the middle of the window
	void SetCursorToCentre();

	//  Tell that mouse object that it has been locked
	void SetMouseLockedCentre(){Input::GetSingleton().SetMouseLocked(gWidth/2,gHeight/2);}

	//  Windows message handler
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

	//  Get a pointer to the window using the handle
	static WindowWizard* GetWindowReference(HWND hWnd);


	HDC GetHandleToDeviceContext(){return hdc;};
	HWND GetHandleToWindow(){return handleToWindow;}
	HGLRC GetRenderingContext(){return hrc;}
	bool SetWindowPixelFormat(HDC hdc);

private:
	void InitializeOpenGL(GLdouble fov);
	void ResizeGLWindow(int width, int height);

	HWND handleToWindow;
	HDC hdc;
	HGLRC hrc;			//hardware RENDERING CONTEXT
	WNDCLASS wcex;
	RECT graphicsRect;
	RECT windowRect;
	GLdouble fieldOfView;

	int gWidth, gHeight;
	static std::map<HWND,WindowWizard*>WindowMap;
	static int cxSource, cySource;

	static const int COLOUR_DEPTH = 16;
};

