#include "controller.h"
#include "input.h"
#include "gameobject.h"


Controller::Controller(GameObject* p):
	parent(p),
	sensitivity(50.0f)
{
}


Controller::~Controller(void)
{
}

void Controller::CheckInputAndAct()
/*-------------------------------------------------------------------------*\
|	Purpose:	Check the input singleton instance for input pertinent		|
|				to object activity function									|
\*-------------------------------------------------------------------------*/
{
	//  Move forward
	if (input.ReportKeyState('W')) {
		parent->MoveLocalDeltaZ(sensitivity*input.GetTimeForLastFrame());
		//  if we press to move forward then turn off autorun
		autoRun = false;
	}
	//  If we're autorunning move forward
	if (autoRun) {
		parent->MoveLocalDeltaZ(sensitivity*input.GetTimeForLastFrame());
	}

	//  Move backwards
	if(input.ReportKeyState('S')) {
		parent->MoveLocalDeltaZ(-sensitivity*input.GetTimeForLastFrame());
		//  if we press to move backwards then turn off autorun
		autoRun = false;
	}

	//  Check for jumping (only jump if not already jumping)
	if (input.ReportKeyPress(VK_SPACE) && jumping == false) {
		parent->GetRigidbody()->SetVelocity(Vector4(0.0f, sensitivity, 0.0f, 1.0f));
		jumping = true;
	}

	//  Middle mouse to toggle autorun
	if (input.ReportMMousePress()) {
		autoRun = !autoRun;
	}


	if (input.GetMouseR()) {
		parent->RotateDeltaY(-(float)input.GetMouseDX()/1000.0);
		parent->RotateLocalDeltaX((float)input.GetMouseDY()/1000.0);
		if (input.ReportKeyState('D'))
			parent->MoveLocalDeltaX(-sensitivity*input.GetTimeForLastFrame());
		if (input.ReportKeyState('A'))
			parent->MoveLocalDeltaX(sensitivity*input.GetTimeForLastFrame());
	} else {
		if (input.ReportKeyState('D')) {
			parent->RotateDeltaY(-sensitivity*input.GetTimeForLastFrame()/5);
		}
		if (input.ReportKeyState('A')) {
			parent->RotateDeltaY(sensitivity*input.GetTimeForLastFrame()/5);
		}
	}
}