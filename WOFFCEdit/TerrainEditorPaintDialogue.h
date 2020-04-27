#pragma once
#include "afxdialogex.h"
#include "resource.h"
#include "afxwin.h"
#include "SceneObject.h"
#include "ToolMain.h"
#include "TerrainTool.h"
#include "TerrainEditorConfirmDialogue.h"
#include <vector>

class TerrainEditorPaintDialogue : public CDialogEx
{
	DECLARE_DYNAMIC(TerrainEditorPaintDialogue)

public:
	TerrainEditorPaintDialogue(CWnd* pParent, ToolMain in_toolSystem);   // modal // takes in out scenegraph in the constructor
	TerrainEditorPaintDialogue(CWnd* pParent = NULL);
	virtual ~TerrainEditorPaintDialogue();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void End();		//kill the dialogue
	afx_msg void Select();	//Item has been selected

	std::vector<std::string> m_texLocations;
	int m_currentSelection;

	DECLARE_MESSAGE_MAP()
public:

	TerrainEditorConfirmDialogue m_TerrainEditorConfirmDialogue;

	// Control variable for more efficient access of the listbox
	CListBox m_listBox;
	virtual BOOL OnInitDialog() override;
	virtual void PostNcDestroy();
	virtual void OnCancel();
	afx_msg void OnBnClickedOk();
	afx_msg void OnLbnSelchangeList1();

	afx_msg void OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void EnableTerrainManip(NMHDR *pNMHDR, LRESULT *pResult);

	void SetText();

	ToolMain* m_toolMain;

	void SetData();

	afx_msg void OnBnClickedButton40001();
	afx_msg void OnBnClickedDELBUTTON();
	afx_msg void OnBnClickedButtonSAVEBUTTON();
	afx_msg void OnDeltaSpin1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLbnSelchangeTexlist();
};


INT_PTR CALLBACK SelectProc(HWND   hwndDlg, UINT   uMsg, WPARAM wParam, LPARAM lParam);
