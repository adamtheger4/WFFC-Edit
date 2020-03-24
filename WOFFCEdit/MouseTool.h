#pragma once
#include <afxext.h>
#include "pch.h"
#include "Game.h"
#include "GameObject.h"
#include "InputCommands.h"
#include "TerrainTool.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

struct ScreenPosToWorldSpaceReturn
{
	DirectX::SimpleMath::Vector3 pos;
	DirectX::SimpleMath::Vector3 direction;
};

enum GrabbedAxis
{
	x,
	y,
	z
};

enum ManipulationType
{
	Position,
	Rotation,
	Scale
};

struct GrabReturns
{
	
};

class MouseTool
{
public:
	MouseTool();
	MouseTool(Game*	m_d3dRenderer, TerrainTool*	in_terrainTool, InputCommands* in_InputCommands);
	~MouseTool();

	void	Update(MSG *msg, CRect WindowRECT);

	//Mouse Variables
	void Grabbing();
	bool Collision();
	bool ClickedObj(DirectX::SimpleMath::Ray ray);
	void LDown(MSG* msg);
	void LUp(MSG* msg);
	void RDown(MSG* msg);
	void RUp(MSG* msg);
	ScreenPosToWorldSpaceReturn ScreenPosToWorldSpace(float x, float y);

	//Game Tool
	Game* m_d3dRenderer;

	TerrainTool* m_terrainTool;

	//Game objects
	std::vector<GameObject*> m_gameGraph;

	//input commands
	InputCommands* m_toolInputCommands;		

	GrabbedAxis m_grabbedAxis;
	ManipulationType m_manipType = ManipulationType::Position;
	GrabReturns m_grabReturns;
	ScreenPosToWorldSpaceReturn m_screenToWorld;

	BoundingBox m_axisBoxX;
	BoundingBox m_axisBoxY;
	BoundingBox m_axisBoxZ;

	//Axis snap values.
	float m_positionSnap = 1.0f;
	float m_rotationSnap = 1.0f;
	float m_scaleSnap = 1.0f;

	//Position
	float	x;
	float	y; 

	float	m_screenWidth;
	float	m_screenHeight;

	std::vector<int> m_selectedObjects; //ID of current Selections

	bool	m_grabbing = false;
	bool	m_windowOpen = false;
	bool	m_pickMultiple = false;

	float	m_grabbedCoords[2];

	float	m_prevX;
	float	m_prevY;
	bool	m_once = false;
};

