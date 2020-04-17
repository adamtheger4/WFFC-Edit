#pragma once
#include "afxdialogex.h"
#include "resource.h"
#include "afxwin.h"
#include "SceneObject.h"
#include "ToolMain.h"
#include "TerrainTool.h"
#include <vector>

class TerrainEditorConfirmDialogue : public CDialogEx
{
	DECLARE_DYNAMIC(TerrainEditorConfirmDialogue)

public:
	TerrainEditorConfirmDialogue(CWnd* pParent = NULL);
	virtual ~TerrainEditorConfirmDialogue();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void End();		//kill the dialogue
	afx_msg void Select();	//Item has been selected

	DECLARE_MESSAGE_MAP()
public:
	// Control variable for more efficient access of the listbox
	virtual BOOL OnInitDialog() override;
	virtual void PostNcDestroy();
	afx_msg void OnBnClickedOk();

	ToolMain* m_toolMain;

	afx_msg void OnBnClickedButtonYes();
	afx_msg void OnBnClickedButtonNo();
};


INT_PTR CALLBACK SelectProc(HWND   hwndDlg, UINT   uMsg, WPARAM wParam, LPARAM lParam);
