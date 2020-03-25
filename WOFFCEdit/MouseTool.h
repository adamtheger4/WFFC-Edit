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
	MouseTool(Game*	m_d3dRenderer, TerrainTool*	in_terrainTool, InputCommands* in_InputCommands, int* screenWidth, int*	screenHeight);
	~MouseTool();

	void Update(MSG *msg, CRect WindowRECT);

	//Mouse Variables
	void LDown(MSG* msg);
	void LUp(MSG* msg);
	void RDown(MSG* msg);
	void RUp(MSG* msg);

	GrabbedAxis m_grabbedAxis;
	ManipulationType m_manipType = ManipulationType::Position;
	GrabReturns m_grabReturns;
	ScreenPosToWorldSpaceReturn m_screenToWorld;

	// Game objects Reference.
	std::vector<GameObject*> m_gameGraph;

	//Axis snap values.
	float m_positionSnap = 1.0f;
	float m_rotationSnap = 1.0f;
	float m_scaleSnap = 1.0f;

	//Position
	float	x;
	float	y; 

	std::vector<int> m_selectedObjects; //ID of current Selections

	bool	m_windowOpen = false;
	bool	m_pickMultiple = false;

private:

	void CamControls();
	bool ClickedObj(DirectX::SimpleMath::Ray ray);
	bool Collision();
	void GrabbingLogic();
	void TerrainToolLogic(Ray in_ray);
	ScreenPosToWorldSpaceReturn ScreenPosToWorldSpace(float x, float y);

	// Game Tool Reference.
	Game* m_d3dRenderer;

	// Terrain Tool Reference.
	TerrainTool* m_terrainTool;

	// Input commands Reference.
	InputCommands* m_toolInputCommands;

	bool	m_grabbing = false;
	float	m_grabbedCoords[2];

	//Reference to Screen dimensions. Stored in Tool main.
	int*	m_screenWidth = 0;
	int*	m_screenHeight = 0;

	//Mouse coords last frame.
	float	m_prevX;
	float	m_prevY;

	//Do once bool used for cam controls.
	bool	m_once = false;

	//Collision boxes used for axis arrows.
	BoundingBox m_axisBoxX;
	BoundingBox m_axisBoxY;
	BoundingBox m_axisBoxZ;

	//Window rectangle.
	CRect m_windowRECT;

	
};

