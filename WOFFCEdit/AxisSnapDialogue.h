#pragma once
#include "afxdialogex.h"
#include "resource.h"
#include "afxwin.h"
#include "SceneObject.h"
#include "ToolMain.h"
#include <vector>

class AxisSnapDialogue : public CDialogEx
{
	DECLARE_DYNAMIC(AxisSnapDialogue)

public:
	AxisSnapDialogue(CWnd* pParent, ToolMain in_toolSystem);   // modal // takes in out scenegraph in the constructor
	AxisSnapDialogue(CWnd* pParent = NULL);
	virtual ~AxisSnapDialogue();

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
	afx_msg void OnLbnSelchangeList1();

	afx_msg void OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin3(NMHDR *pNMHDR, LRESULT *pResult);

	void SetText();

	ToolMain* m_toolMain;

	// Data member in your dialog class 
	CStatic m_Text1;

	afx_msg void OnStnClickedScatext();
};


INT_PTR CALLBACK SelectProc(HWND   hwndDlg, UINT   uMsg, WPARAM wParam, LPARAM lParam);
