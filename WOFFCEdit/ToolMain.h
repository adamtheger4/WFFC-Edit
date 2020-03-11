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

	//inline std::vector<SceneObject> GetSceneGraph() { return GameGraphToSceneGraph(m_gameGraph); }

public:	//variables
	//std::vector<SceneObject>    m_sceneGraph;	//our scenegraph storing all the objects in the current chunk
	std::vector<GameObject>    m_gameGraph;
	ChunkObject					m_chunk;		//our landscape chunk
	int m_selectedObject;						//ID of current Selection

private:	//methods
	void	onContentAdded();

	std::vector<SceneObject> GameGraphToSceneGraph(std::vector<GameObject> in_gameGraph);

	ScreenPosToWorldSpaceReturn ScreenPosToWorldSpace(int x, int y);
		
private:	//variables
	HWND	m_toolHandle;		//Handle to the  window
	Game	m_d3dRenderer;		//Instance of D3D rendering system for our tool
	InputCommands m_toolInputCommands;		//input commands that we want to use and possibly pass over to the renderer
	CRect	WindowRECT;		//Window area rectangle. 
	char	m_keyArray[256];
	sqlite3 *m_databaseConnection;	//sqldatabase handle

	//Mouse Variables
	bool MouseCollision();
	bool MouseClickedObj(DirectX::SimpleMath::Ray ray);
	void MouseLDown(MSG* msg);
	void MouseLUp(MSG* msg);
	void MouseRDown(MSG* msg);
	void MouseRUp(MSG* msg);

	bool	mouseGrabbing = false;
	GrabbedAxis grabbedAxis;
	float	mouseGrabbedCoords[2];
	bool	m_once = false;
	float	mouse_x;
	float	mouse_y;

	int m_width;		//dimensions passed to directX
	int m_height;
	int m_currentChunk;			//the current chunk of thedatabase that we are operating on.  Dictates loading and saving. 	

	DirectX::BoundingBox m_axisBoxX;
	DirectX::BoundingBox m_axisBoxY;
	DirectX::BoundingBox m_axisBoxZ;
};
