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

	void SaveHeightmap();
	void SavePreviousHeightmap();
	void UndoHeightmapChanges();

	void LoadTextureToPaint(std::string inPath, int texIndex);
	void SaveTerrainTextures();
	void SavePreviousTerrainTextures();
	void SetTerrainLayerIndex(int index);
	void UndoTerrainPaintChanges();

	int GetTerrainLayerTextureIndex();
	int GetTerrainLayerIndex();

public:	//variables
	//std::vector<SceneObject>    m_sceneGraph;	

	//our scenegraph storing all the objects in the current chunk
	std::vector<GameObject>    m_gameGraph;
	ChunkObject					m_chunk;		//our landscape chunk

	std::vector <std::vector<GameObject>> m_undoGameGraph; //Previous game graphs for undoing actions
	std::vector <ChunkObject>			  m_undoChunk;	//previous landscape chunk for undoing actions
	std::vector <std::vector<GameObject>> m_redoGameGraph; //Previous game graph for redoing actions
	std::vector <ChunkObject>			  m_redoChunk;	//previous landscape chunk for redoing actions

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

	void DeleteObjects(std::vector<int> objectIDs); // marks in objects to be deleted at end of tick in toolmain.
	void CopyObjects(std::vector<GameObject> in_gameObjects); // stores the in objects to be pasted.
	std::vector<GameObject> PasteObjects();				   // Pastes the stored object (adds new object to game Graph).
	std::vector<GameObject> copyObjects; // the object currently stored in the clipboard to be copied.

	//Destroys any gameGraph objects at the end of the tick.
	void DestroyObjects();
	//Whether to call Destroy objects at the end of the next tick.
	bool bDoDestroy = false;

	void SaveLastAction();
	void UndoAction();
	void RedoAction();

	int m_width;		//dimensions passed to directX
	int m_height;
	int m_currentChunk;			//the current chunk of thedatabase that we are operating on.  Dictates loading and saving. 	
};
