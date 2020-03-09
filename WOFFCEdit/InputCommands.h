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
	bool rollLeft;
	bool rollRight;

	float camRotate = 1;
	float m_x;
	float m_y;
};
