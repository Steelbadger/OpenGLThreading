#include "application.h"

#include <windows.h>
#include <stdio.h>
#include <mmsystem.h>

int main (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int nCmdShow)
/*-------------------------------------------------------------------------*\
|	Purpose:	program entry point											|
|																			|
|	Parameters:	Handed in by executable										|
|																			|
|	Returns:	process return.														|
|																			|
\*-------------------------------------------------------------------------*/
{	
    MSG msg;

	//  Create the application
	Application myApp;
	myApp.Initialize(hInstance);

	while (true) {	
		//  handle messages
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE)) {
		    if (msg.message==WM_QUIT)
				break;
			TranslateMessage (&msg);							
			DispatchMessage (&msg);
		} else {
		//  Update application
			myApp.MainLoop();
		}
    }
	return msg.wParam ;										
}