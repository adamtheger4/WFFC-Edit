#pragma once

struct InputCommands
{
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

	bool mouseRotation;
	float camRotateX = 1;
	float camRotateY = 1;
	float camMove = 1;

	int intersects = 0;
};
