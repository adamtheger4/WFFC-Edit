#include "AxisSnapDialogue.h"
#include <string>
#include "stdafx.h"

IMPLEMENT_DYNAMIC(AxisSnapDialogue, CDialogEx)

//Message map.  Just like MFCMAIN.cpp.  This is where we catch button presses etc and point them to a handy dandy method.
BEGIN_MESSAGE_MAP(AxisSnapDialogue, CDialogEx)
	ON_COMMAND(IDOK, &AxisSnapDialogue::End)					//ok button
	ON_BN_CLICKED(IDOK, &AxisSnapDialogue::End)
	//ON_EN_CHANGE(IDC_EDIT1, &AxisSnapDialogue::UpdateEdit1)
	//ON_EN_CHANGE(IDC_EDIT2, &AxisSnapDialogue::UpdateEdit2)
	//ON_EN_CHANGE(IDC_EDIT3, &AxisSnapDialogue::UpdateEdit3)
//	ON_LBN_SELCHANGE(IDC_LIST1, &AxisSnapDialogue::Select)	//listbox
ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &AxisSnapDialogue::OnDeltaposSpin1)
ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, &AxisSnapDialogue::OnDeltaposSpin2)
ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN3, &AxisSnapDialogue::OnDeltaposSpin3)
END_MESSAGE_MAP()

AxisSnapDialogue::AxisSnapDialogue(CWnd * pParent, ToolMain in_toolSystem)
	: CDialogEx(IDD_DIALOG2, pParent)
{
	m_toolMain = &in_toolSystem;
}


AxisSnapDialogue::AxisSnapDialogue(CWnd * pParent)
	: CDialogEx(IDD_DIALOG2, pParent)
{
}

AxisSnapDialogue::~AxisSnapDialogue()
{
}

void AxisSnapDialogue::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_LIST1, m_listBox);
}

void AxisSnapDialogue::End()
{
	m_toolMain->windowOpen = false;
	DestroyWindow();	//destory the window properly.  INcluding the links and pointers created.  THis is so the dialogue can start again. 	
}

BOOL AxisSnapDialogue::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//uncomment for modal only
/*	//roll through all the objects in the scene graph and put an entry for each in the listbox
	int numSceneObjects = m_sceneGraph->size();
	for (size_t i = 0; i < numSceneObjects; i++)
	{
		//easily possible to make the data string presented more complex. showing other columns.
		std::wstring listBoxEntry = std::to_wstring(m_sceneGraph->at(i).ID);
		m_listBox.AddString(listBoxEntry.c_str());
	}*/

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void AxisSnapDialogue::PostNcDestroy()
{
}




// SelectDialogue message handlers callback   - We only need this if the dailogue is being setup-with createDialogue().  We are doing
//it manually so its better to use the messagemap
/*INT_PTR CALLBACK SelectProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
		//	EndDialog(hwndDlg, wParam);
			DestroyWindow(hwndDlg);
			return TRUE;


		case IDCANCEL:
			EndDialog(hwndDlg, wParam);
			return TRUE;
			break;
		}
	}

	return INT_PTR();
}*/


void AxisSnapDialogue::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}

void AxisSnapDialogue::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	m_toolMain->m_positionSnap -= pNMUpDown->iDelta;

	CString t;
	t.Format(_T("%d"), (int)m_toolMain->m_positionSnap);

	CWnd* pWnd = GetDlgItem(POSTEXT);
	pWnd->SetWindowText(t);
}

void AxisSnapDialogue::OnDeltaposSpin2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	m_toolMain->m_rotationSnap -= pNMUpDown->iDelta;

	CString t;
	t.Format(_T("%d"), (int)m_toolMain->m_rotationSnap);

	CWnd* pWnd = GetDlgItem(ROTTEXT);
	pWnd->SetWindowText(t);
}

void AxisSnapDialogue::OnDeltaposSpin3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	m_toolMain->m_scaleSnap -= pNMUpDown->iDelta;

	CString t;
	t.Format(_T("%d"), (int)m_toolMain->m_scaleSnap);

	CWnd* pWnd = GetDlgItem(SCATEXT);
	pWnd->SetWindowText(t);
}

void AxisSnapDialogue::SetText()
{
	//Set the text boxes to display the current snap values.
	CString t;
	t.Format(_T("%d"), (int)m_toolMain->m_positionSnap);

	CWnd* pWnd = GetDlgItem(POSTEXT);
	pWnd->SetWindowText(t);

	t.Format(_T("%d"), (int)m_toolMain->m_rotationSnap);

	pWnd = GetDlgItem(ROTTEXT);
	pWnd->SetWindowText(t);

	t.Format(_T("%d"), (int)m_toolMain->m_scaleSnap);

	pWnd = GetDlgItem(SCATEXT);
	pWnd->SetWindowText(t);
}


void AxisSnapDialogue::OnStnClickedScatext()
{
	// TODO: Add your control notification handler code here
}
