#pragma once

struct InputCommands
{
	bool mouseControls;

	//Camera controls
	bool forward;
	bool back;
	bool right;
	bool left;
	bool up;
	bool down;
	bool rotLeft;
	bool rotRight;
	bool rotUp;
	bool rotDown;

	float camRotateX = 1;
	float camRotateY = 1;
	float camMove = 1;

	//Obj movement
	bool moveObjForward;
	bool moveObjBack;
	bool moveObjRight;
	bool moveObjLeft;
	bool moveObjUp;
	bool moveObjDown;

	float objMove = 1.0f;
};
