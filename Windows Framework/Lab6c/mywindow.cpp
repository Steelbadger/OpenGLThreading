#include "mywindow.h"
#include "input.h"

std::map<HWND, WindowWizard*> WindowWizard::WindowMap;
int WindowWizard::cxSource;
int WindowWizard::cySource;

WindowWizard::WindowWizard(void)
{
}


WindowWizard::WindowWizard(LPSTR WindowName, int width, int height, HINSTANCE hInstance)
{
	WindowCreate(WindowName, width, height, (WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN), 0, hInstance);
}


WindowWizard::WindowWizard(LPSTR WindowName, int width, int height, HINSTANCE hInstance, bool FullScreen, DWORD Style)
{
	WindowCreate(WindowName, width, height, Style, FullScreen, hInstance);
}


WindowWizard::~WindowWizard(void)
{
	if (hrc)
	{
		wglMakeCurrent(NULL, NULL);	// free rendering memory
		wglDeleteContext(hrc);		// Delete our OpenGL Rendering Context
	}

	if (hdc) 
		ReleaseDC(handleToWindow, hdc);			// Release our HDC from memory

	UnregisterClass(wcex.lpszClassName, wcex.hInstance);// Free the window class

	PostQuitMessage (0);		// Post a QUIT message to the window
}

void WindowWizard::SetCursorToCentre()
/*-------------------------------------------------------------------------*\
|	Purpose:	Move the cursor onscreen to the middle of the window		|
\*-------------------------------------------------------------------------*/
{
	SetCursorPos(gWidth/2+windowRect.left,gHeight/2+windowRect.top);
}

void WindowWizard::InitializeGraphics(GLdouble fov)
/*-------------------------------------------------------------------------*\
|	Purpose:	Do the graphics initialisation stuff, ideally should be		|
|				removed from the window class (move to renderer)			|
|	Parameters:	The field of view of the camera (largely unecessary with	|
|				new custom camera)											|		
\*-------------------------------------------------------------------------*/
{
	GetClientRect(handleToWindow, &graphicsRect);	//get rect into our handy global rect
	GetWindowRect(handleToWindow, &windowRect);
	InitializeOpenGL(fov);						// initialise openGL

	//OpenGL settings
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

	//Also, do any other setting ov variables here for your app if you wish. 
	glEnable(GL_TEXTURE_2D);							//Enable texturing!
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);	//specify texture calculation

	//Also, do any other setting ov variables here for your app if you wish. 
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);// Linear Filtering
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);// Linear Filtering
}


void WindowWizard::InitializeOpenGL(GLdouble fov)
/*-------------------------------------------------------------------------*\
|	Purpose:	Create the OpenGL render context and set the OpenGL window	|
|				size														|
|																			|
|	Parameters:	Field of view of the camera, again largely unecessary with	|
|				new custom camera											|
\*-------------------------------------------------------------------------*/
{
    hdc = GetDC(handleToWindow);//  sets  global HDC
	fieldOfView = fov;

    if (!SetWindowPixelFormat(hdc))//  sets  pixel format
        PostQuitMessage (0);


    hrc = wglCreateContext(hdc);	//  creates  rendering context from  hdc
    wglMakeCurrent(hdc, hrc);		//	Use this HRC.

	ResizeGLWindow(graphicsRect.right, graphicsRect.bottom);	// Setup the Screen
}

void WindowWizard::FlipBuffers()
/*-------------------------------------------------------------------------*\
|	Purpose:	Swap the front and back buffers (draw current frame to		|
|				screen)														|
\*-------------------------------------------------------------------------*/
{
	SwapBuffers(hdc);
}

void WindowWizard::PrepareForDrawing()
/*-------------------------------------------------------------------------*\
|	Purpose:	Clear the OpenGL back buffer and depth buffer ready for		|
|				drawing														|
\*-------------------------------------------------------------------------*/
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);// Clear The Screen And The Depth Buffer
}


void WindowWizard::OnResize()
/*-------------------------------------------------------------------------*\
|	Purpose:	Update the saved size of the window after a resize and 		|
|				resize the OpenGL viewport to fit the new window			|
\*-------------------------------------------------------------------------*/
{
	GetClientRect(handleToWindow, &graphicsRect);
	GetWindowRect(handleToWindow, &windowRect);
	ResizeGLWindow(graphicsRect.right, graphicsRect.bottom);
}

void WindowWizard::OnMove()
/*-------------------------------------------------------------------------*\
|	Purpose:	Update the saved position of the window with the new  		|
|				position													|
\*-------------------------------------------------------------------------*/
{
	GetWindowRect(handleToWindow, &windowRect);
}


void WindowWizard::ResizeGLWindow(int width, int height)
/*-------------------------------------------------------------------------*\
|	Purpose:	Mostly useless with new programmable pipeline (only thing	|
|				that does anything is setting height/width and glViewport)	|
\*-------------------------------------------------------------------------*/
{
	gHeight = height;
	gWidth = width;
	if (height==0)// Prevent A Divide By Zero error
	{
		height=1;// Make the Height Equal One
	}

	glViewport(0,0,width,height);		//  Create the OpenGL 'window' to fill the actual window

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//calculate aspect ratio
	gluPerspective(fieldOfView,(GLfloat)width/(GLfloat)height, 1 ,1000.0f);

	glMatrixMode(GL_MODELVIEW);// Select The Modelview Matrix
	glLoadIdentity();// Reset The Modelview Matrix
}


bool WindowWizard::SetWindowPixelFormat(HDC hdc)
/*-------------------------------------------------------------------------*\
|	Purpose:	Mostly useless with new programmable pipeline (only thing	|
|				that does anything is setting height/width and glViewport)	|
\*-------------------------------------------------------------------------*/
{ 
    PIXELFORMATDESCRIPTOR pfd = {0}; 
    int pixelformat; 
 
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);	// Set the size of the structure
    pfd.nVersion = 1;							// Always set this to 1
	// Pass in the appropriate OpenGL flags
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER; 
    pfd.dwLayerMask = PFD_MAIN_PLANE;			// standard mask (this is ignored anyway)
    pfd.iPixelType = PFD_TYPE_RGBA;				// RGB and Alpha pixel type
    pfd.cColorBits = COLOUR_DEPTH;				// Here we use our #define for the color bits
    pfd.cDepthBits = COLOUR_DEPTH;				// Ignored for RBA
    pfd.cAccumBits = 0;							// nothing for accumulation
    pfd.cStencilBits = 0;						// nothing for stencil
 
	//Gets a best match on the pixel format as passed in from device
    if ( (pixelformat = ChoosePixelFormat(hdc, &pfd)) == false ) 
    { 
        MessageBox(NULL, "ChoosePixelFormat failed", "Error", MB_OK); 
        return false; 
    } 
 
	//sets the pixel format if its ok. 
    if (SetPixelFormat(hdc, pixelformat, &pfd) == false) 
    { 
        MessageBox(NULL, "SetPixelFormat failed", "Error", MB_OK); 
        return false; 
    } 
 
    return true;
}


void WindowWizard::WindowCreate(LPSTR strWindowName, int width, int height, DWORD dwStyle, bool bFullScreen, HINSTANCE hInstance)
/*-------------------------------------------------------------------------*\
|	Purpose:	Create the Win32 Window										|
|																			|
|	Parameters:	The title string of the window, the width and height of		|
|				the window, the styles of the window (see MSDN)				|
|				boolean for fullscreen window and the application			|
|				instance.													|
\*-------------------------------------------------------------------------*/
{


	memset(&wcex, 0, sizeof(WNDCLASS));
	wcex.style			= CS_HREDRAW | CS_VREDRAW;		
	wcex.lpfnWndProc	= &WindowWizard::WndProc;		
	wcex.hInstance		= hInstance;						
	wcex.hIcon			= LoadIcon(NULL, IDI_APPLICATION);; 
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);		
	wcex.hbrBackground	= (HBRUSH) (COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;	
	wcex.lpszClassName	= strWindowName;	

	
	RegisterClass(&wcex);// Register the class

	//dwStyle = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	//Set the Client area of the window to be our resolution.
	RECT glwindow;
	glwindow.left		= 0;		
	glwindow.right		= width;	
	glwindow.top		= 0;		
	glwindow.bottom		= height;	

	AdjustWindowRect( &glwindow, dwStyle, false);

	//Create the window
	handleToWindow = CreateWindow(	strWindowName, 
							strWindowName, 
							dwStyle, 
							0, 
							0,
							glwindow.right  - glwindow.left,
							glwindow.bottom - glwindow.top, 
							NULL,
							NULL,
							hInstance,
							NULL
							);

	if(!handleToWindow) {
		MessageBox(NULL, "Could Not Get Handle To Window", "Error", MB_OK); // If we couldn't get a handle, return NULL
		PostQuitMessage (0);
	}
	WindowMap[handleToWindow] = this;

	ShowWindow(handleToWindow, SW_SHOWNORMAL);	
	UpdateWindow(handleToWindow);					
	SetFocus(handleToWindow);

}

WindowWizard* WindowWizard::GetWindowReference(HWND hwnd)
/*-------------------------------------------------------------------------*\
|	Purpose:	Use the window handle to return a pointer to the window		|
|				object that tracks that window								|
|																			|
|	Parameters: Handle to window of interest								|
|																			|
|	Returns:	Pointer to the window object of interest					|
\*-------------------------------------------------------------------------*/
{
	return WindowMap[hwnd];
}


LRESULT CALLBACK WindowWizard::WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
/*-------------------------------------------------------------------------*\
|	Purpose:	The message handling function, this function is static 		|
|				due to constraints within the windows api, finds the 		|
|				window concerned by using the handle to the window in a map	|
|																			|
|	Parameters: Windows message parameters									|
\*-------------------------------------------------------------------------*/
{
    if(message == WM_CREATE)		//  Create message gets sent before the window is finished being made
    {								//  So catch it here
		cxSource = GetSystemMetrics (SM_CXSIZEFRAME) + GetSystemMetrics (SM_CXSIZE);
		cySource = GetSystemMetrics (SM_CYSIZEFRAME) + GetSystemMetrics (SM_CYCAPTION);
    }
    else
    {
		WindowWizard* pWindow = WindowMap[hwnd];		//  Find the window of interest using the handle
		
        switch (message)
        {
            case WM_PAINT:
                break;
            case WM_COMMAND:
				break;
			case WM_SIZE:
				pWindow->OnResize();
			case WM_MOVE:
				GetWindowRect(hwnd,&pWindow->windowRect);
            case WM_LBUTTONUP:
            case WM_LBUTTONDOWN:
			case WM_RBUTTONDOWN:
            case WM_RBUTTONUP:
			case WM_MBUTTONDOWN:
			case WM_MBUTTONUP:
			case WM_MOUSEWHEEL:
            case WM_MOUSEMOVE:
			case WM_KEYDOWN:
			case WM_KEYUP:
				input.Message(message, wParam, lParam);
				break;
            case WM_DESTROY:
                PostQuitMessage(0);
                return 0;
        }
    }
    return DefWindowProc (hwnd, message, wParam, lParam) ;
}
