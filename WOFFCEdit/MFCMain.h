#pragma once

#include <afxwin.h> 
#include <afxext.h>
#include <afx.h>
#include "pch.h"
#include "Game.h"
#include "ToolMain.h"
#include "resource.h"
#include "MFCFrame.h"
#include "SelectDialogue.h"
#include "AxisSnapDialogue.h"
#include "TerrainEditorDialogue.h"
#include "TerrainEditorPaintDialogue.h"
#include "TerrainEditorConfirmDialogue.h"


class MFCMain : public CWinApp 
{
public:
	MFCMain();
	~MFCMain();
	BOOL InitInstance();
	int  Run();

private:

	CMyFrame * m_frame;	//handle to the frame where all our UI is
	HWND m_toolHandle;	//Handle to the MFC window
	ToolMain m_ToolSystem;	//Instance of Tool System that we interface to. 
	CRect WindowRECT;	//Window area rectangle. 
	SelectDialogue m_ToolSelectDialogue;			//for modeless dialogue, declare it here
	AxisSnapDialogue m_ToolAxisSnapDialogue;
	TerrainEditorDialogue m_TerrainEditorDialogue;
	TerrainEditorPaintDialogue m_TerrainEditorPaintDialogue;

	int m_width;		
	int m_height;
	
	//Interface funtions for menu and toolbar etc requires
	afx_msg void MenuFileQuit();
	afx_msg void MenuFileSaveTerrain();
	afx_msg void MenuEditSelect();
	afx_msg void SetAxisSnapValues();
	afx_msg void OpenTerrainDialogue();
	afx_msg	void ToolBarButton1();
	afx_msg	inline void SetToolManipTypePos() { m_ToolSystem.m_mouseTool.m_manipType = ManipulationType::Position;}
	afx_msg	inline void SetToolManipTypeRot() { m_ToolSystem.m_mouseTool.m_manipType = ManipulationType::Rotation;}
	afx_msg	inline void SetToolManipTypeSca() { m_ToolSystem.m_mouseTool.m_manipType = ManipulationType::Scale;}


	DECLARE_MESSAGE_MAP()	// required macro for message map functionality  One per class
};
