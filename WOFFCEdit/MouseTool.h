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
	MouseTool(Game*	m_d3dRenderer, TerrainTool*	in_terrainTool);
	~MouseTool();

	void	Update(MSG *msg, CRect WindowRECT);

	//Mouse Variables
	void MouseGrabbing();
	bool MouseCollision();
	bool MouseClickedObj(DirectX::SimpleMath::Ray ray);
	void MouseLDown(MSG* msg);
	void MouseLUp(MSG* msg);
	void MouseRDown(MSG* msg);
	void MouseRUp(MSG* msg);
	ScreenPosToWorldSpaceReturn ScreenPosToWorldSpace();

	//Game Tool
	Game* m_d3dRenderer;

	TerrainTool* m_terrainTool;

	//Game objects
	std::vector<GameObject> m_gameGraph;

	//input commands
	InputCommands* m_toolInputCommands;		

	GrabbedAxis m_grabbedAxis;
	ManipulationType m_manipType = ManipulationType::Position;
	GrabReturns m_grabReturns;
	ScreenPosToWorldSpaceReturn m_screenToWorld;

	BoundingBox* m_axisBoxX;
	BoundingBox* m_axisBoxY;
	BoundingBox* m_axisBoxZ;

	//Axis snap values.
	float m_positionSnap = 1.0f;
	float m_rotationSnap = 1.0f;
	float m_scaleSnap = 1.0f;

	//Position
	float	x;
	float	y; 

	float	m_screenWidth;
	float	m_screenHeight;

	int m_selectedObject; //ID of current Selection

	bool	m_grabbing = false;
	bool	m_windowOpen = false;

	float	m_grabbedCoords[2];

	float	m_prevX;
	float	m_prevY;
	bool	m_once = false;
};

