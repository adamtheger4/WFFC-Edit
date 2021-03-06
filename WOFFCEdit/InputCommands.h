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
	bool scforward;
	bool scback;

	float camRotateX = 1;
	float camRotateY = 1;
	float camMove = 1;
	float camMovementScalar = 1.0;
	float objMove = 1.0f;
};
