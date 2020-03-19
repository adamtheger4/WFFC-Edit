#pragma once

#include <afxext.h>
#include "pch.h"
#include "Game.h"
#include "sqlite3.h"
#include "GameObject.h"
#include "InputCommands.h"
#include <vector>

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

class ToolMain
{
public: //methods
	ToolMain();
	~ToolMain();

	//onAction - These are the interface to MFC
	int		getCurrentSelectionID();										//returns the selection number of currently selected object so that It can be displayed.
	void	onActionInitialise(HWND handle, int width, int height);			//Passes through handle and hieght and width and initialises DirectX renderer and SQL LITE
	void	onActionFocusCamera();
	void	onActionLoad();													//load the current chunk
	afx_msg	void	onActionSave();											//save the current chunk
	afx_msg void	onActionSaveTerrain();									//save chunk geometry

	void	Tick(MSG *msg);
	void	UpdateInput(MSG *msg);
	void	MouseUpdate();

	inline std::vector<SceneObject> GetSceneGraph() { return GameGraphToSceneGraph(m_gameGraph); }

	inline void SetManipType(ManipulationType in_type) { m_manipType = in_type; }

	//Axis snap values.
	float m_positionSnap = 1.0f;
	float m_rotationSnap = 1.0f;
	float m_scaleSnap = 1.0f;

	DirectX::XMFLOAT3 terrainManipulationOffset;
	bool	mouseTerrainTool = false;
	bool	mouseTerrainToolDig = false;
	void	EnableTerrainText(bool enable);
	void	UpdateTerrainHeightText();

	bool	windowOpen = false;

public:	//variables
	//std::vector<SceneObject>    m_sceneGraph;	//our scenegraph storing all the objects in the current chunk
	std::vector<GameObject>    m_gameGraph;
	ChunkObject					m_chunk;		//our landscape chunk
	int m_selectedObject;						//ID of current Selection

private:	//methods
	void	onContentAdded();

	std::vector<SceneObject> GameGraphToSceneGraph(std::vector<GameObject> in_gameGraph);

	ScreenPosToWorldSpaceReturn ScreenPosToWorldSpace(float x, float y);
		
private:	//variables
	HWND	m_toolHandle;		//Handle to the  window
	Game	m_d3dRenderer;		//Instance of D3D rendering system for our tool
	InputCommands m_toolInputCommands;		//input commands that we want to use and possibly pass over to the renderer
	CRect	WindowRECT;		//Window area rectangle. 
	char	m_keyArray[256];
	sqlite3 *m_databaseConnection;	//sqldatabase handle

	//Mouse Variables
	void MouseGrabbing();
	bool MouseCollision();
	bool MouseClickedObj(DirectX::SimpleMath::Ray ray);
	void MouseLDown(MSG* msg);
	void MouseLUp(MSG* msg);
	void MouseRDown(MSG* msg);
	void MouseRUp(MSG* msg);

	GrabbedAxis grabbedAxis;
	RayToDisplayChunkReturn mouseTerrainManipReturn;
	bool	mouseGrabbing = false;
	bool	mouseTerrainToolActive = false;
	float	mouseGrabbedCoords[2];
	float	mouse_x;
	float	mouse_y;
	float	prevMouse_x;
	float	prevMouse_y;
	bool	m_once = false;

	//Terrain tool toggle dig do once.
	bool	m_Tonce = false;

	int m_width;		//dimensions passed to directX
	int m_height;
	int m_currentChunk;			//the current chunk of thedatabase that we are operating on.  Dictates loading and saving. 	

	DirectX::BoundingOrientedBox m_axisBoxX;
	DirectX::BoundingOrientedBox m_axisBoxY;
	DirectX::BoundingOrientedBox m_axisBoxZ;

	ManipulationType m_manipType = ManipulationType::Position;
};
