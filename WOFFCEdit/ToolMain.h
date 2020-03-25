#pragma once

#include <afxext.h>
#include "pch.h"
#include "Game.h"
#include "sqlite3.h"
#include "GameObject.h"
#include "InputCommands.h"
#include "TerrainTool.h"
#include "MouseTool.h"
#include <vector>

class ToolMain
{
public: //methods
	ToolMain();
	~ToolMain();

	//onAction - These are the interface to MFC
	void	onActionInitialise(HWND handle, int width, int height);			//Passes through handle and hieght and width and initialises DirectX renderer and SQL LITE
	void	onActionFocusCamera();
	void	onActionLoad();													//load the current chunk
	afx_msg	void	onActionSave();											//save the current chunk
	afx_msg void	onActionSaveTerrain();									//save chunk geometry

	void	Tick(MSG *msg);
	void	UpdateInput(MSG *msg);

	inline std::vector<SceneObject> GetSceneGraph() { return GameGraphToSceneGraph(m_gameGraph); }

	TerrainTool m_terrainTool;
	MouseTool m_mouseTool = MouseTool(&m_d3dRenderer, &m_terrainTool, &m_toolInputCommands, &m_width, &m_height);

	void	EnableTerrainText(bool enable);
	void	UpdateTerrainText();

	bool	windowOpen = false;

public:	//variables
	//std::vector<SceneObject>    m_sceneGraph;	//our scenegraph storing all the objects in the current chunk
	std::vector<GameObject>    m_gameGraph;
	ChunkObject					m_chunk;		//our landscape chunk

private:	//methods
	void	onContentAdded();

	std::vector<SceneObject> GameGraphToSceneGraph(std::vector<GameObject> in_gameGraph);
		
private:	//variables
	HWND	m_toolHandle;		//Handle to the  window
	Game	m_d3dRenderer;		//Instance of D3D rendering system for our tool
	InputCommands m_toolInputCommands;		//input commands that we want to use and possibly pass over to the renderer
	CRect	WindowRECT;		//Window area rectangle. 
	char	m_keyArray[256];
	sqlite3 *m_databaseConnection;	//sqldatabase handle

	void CopyObject(GameObject in_gameObject); // stores the in object to be pasted.
	GameObject PasteObject();				   // Pastes the stored object (adds new object to game Graph).
	GameObject copyObject; // the object currently stored in the clipboard to be copied.

	int m_width;		//dimensions passed to directX
	int m_height;
	int m_currentChunk;			//the current chunk of thedatabase that we are operating on.  Dictates loading and saving. 	
};
