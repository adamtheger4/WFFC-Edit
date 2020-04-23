#pragma once
#include "afxdialogex.h"
#include "resource.h"
#include "afxwin.h"
#include "SceneObject.h"
#include "ToolMain.h"
#include "TerrainTool.h"
#include "TerrainEditorConfirmDialogue.h"
#include <vector>

class TerrainEditorDialogue : public CDialogEx
{
	DECLARE_DYNAMIC(TerrainEditorDialogue)

public:
	TerrainEditorDialogue(CWnd* pParent, ToolMain in_toolSystem);   // modal // takes in out scenegraph in the constructor
	TerrainEditorDialogue(CWnd* pParent = NULL);
	virtual ~TerrainEditorDialogue();

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

	TerrainEditorConfirmDialogue m_TerrainEditorConfirmDialogue;

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
	afx_msg void OnBnClickedButton40002();
	afx_msg void OnBnClickedButton40003();
	afx_msg void OnBnClickedButton40004();
	afx_msg void OnBnClickedButtonSAVEBUTTON();
	afx_msg void OnDeltaSpin1(NMHDR *pNMHDR, LRESULT *pResult);
};


INT_PTR CALLBACK SelectProc(HWND   hwndDlg, UINT   uMsg, WPARAM wParam, LPARAM lParam);
