#include "TerrainEditorDialogue.h"
#include <string>
#include "stdafx.h"

IMPLEMENT_DYNAMIC(TerrainEditorDialogue, CDialogEx)

//Message map.  Just like MFCMAIN.cpp.  This is where we catch button presses etc and point them to a handy dandy method.
BEGIN_MESSAGE_MAP(TerrainEditorDialogue, CDialogEx)
	ON_COMMAND(IDOK, &TerrainEditorDialogue::End)					//ok button
	ON_BN_CLICKED(IDOK, &TerrainEditorDialogue::End)
	ON_NOTIFY(UDN_DELTAPOS, IDC_HEIGHTSPIN, &TerrainEditorDialogue::OnDeltaSpin1)
	ON_BN_CLICKED(ID_BUTTON40001, &TerrainEditorDialogue::OnBnClickedButton40001)
	ON_BN_CLICKED(ID_BUTTON40002, &TerrainEditorDialogue::OnBnClickedButton40002)
	ON_BN_CLICKED(ID_BUTTON40003, &TerrainEditorDialogue::OnBnClickedButton40003)
	ON_BN_CLICKED(ID_BUTTON40004, &TerrainEditorDialogue::OnBnClickedButton40004)
	ON_BN_CLICKED(SAVEBUTTON, &TerrainEditorDialogue::OnBnClickedButtonSAVEBUTTON)
END_MESSAGE_MAP()

TerrainEditorDialogue::TerrainEditorDialogue(CWnd * pParent, ToolMain in_toolSystem)
	: CDialogEx(IDD_DIALOG3, pParent)
{
	m_toolMain = &in_toolSystem;
}


TerrainEditorDialogue::TerrainEditorDialogue(CWnd * pParent)
	: CDialogEx(IDD_DIALOG3, pParent)
{
}

TerrainEditorDialogue::~TerrainEditorDialogue()
{
	DestroyWindow();
}

void TerrainEditorDialogue::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_LIST1, m_listBox);
}

void TerrainEditorDialogue::End()
{
	//m_toolMain->windowOpen = false;

	DestroyWindow();	//destory the window properly.  INcluding the links and pointers created.  THis is so the dialogue can start again. 
}

BOOL TerrainEditorDialogue::OnInitDialog()
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

void TerrainEditorDialogue::PostNcDestroy()
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


void TerrainEditorDialogue::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}

void TerrainEditorDialogue::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	m_toolMain->m_mouseTool.m_positionSnap -= pNMUpDown->iDelta;

	CString t;
	t.Format(_T("%d"), (int)m_toolMain->m_mouseTool.m_positionSnap);

	CWnd* pWnd = GetDlgItem(POSTEXT);
	pWnd->SetWindowText(t);
}

void TerrainEditorDialogue::EnableTerrainManip(NMHDR * pNMHDR, LRESULT * pResult)
{

}


void TerrainEditorDialogue::SetText()
{
	//Set the text boxes to display the current snap values.
	//CString t;
	//t.Format(_T("%d"), (int)m_toolMain->m_positionSnap);

	//CWnd* pWnd = GetDlgItem(POSTEXT);
	//pWnd->SetWindowText(t);

	//t.Format(_T("%d"), (int)m_toolMain->m_rotationSnap);

	//pWnd = GetDlgItem(ROTTEXT);
	//pWnd->SetWindowText(t);

	//t.Format(_T("%d"), (int)m_toolMain->m_scaleSnap);

	//pWnd = GetDlgItem(SCATEXT);
	//pWnd->SetWindowText(t);
}


void TerrainEditorDialogue::SetData()
{
	CButton *m_ctlCheck1 = (CButton*)GetDlgItem(ID_BUTTON40001);
	if (m_toolMain->m_terrainTool.GetEnable())
	{
		m_ctlCheck1->SetCheck(1);
	}
	else
	{
		m_ctlCheck1->SetCheck(0);
	}

	CButton *m_ctlCheck2 = (CButton*)GetDlgItem(ID_BUTTON40002);
	if (m_toolMain->m_terrainTool.smoothSculpt)
	{
		m_ctlCheck2->SetCheck(1);
	}
	else
	{
		m_ctlCheck2->SetCheck(0);
	}

	CButton *m_ctlCheck3 = (CButton*)GetDlgItem(ID_BUTTON40003);
	CButton *m_ctlCheck4 = (CButton*)GetDlgItem(ID_BUTTON40004);
	if (m_toolMain->m_terrainTool.m_terrainSculptMode == TerrainSculptMode::Hill)
	{
		m_ctlCheck3->SetCheck(1);
		m_ctlCheck4->SetCheck(0);
	}
	else
	{
		m_ctlCheck3->SetCheck(0);
		m_ctlCheck4->SetCheck(1);
	}

	CString t;
	t.Format(_T("%d"), (int)m_toolMain->m_terrainTool.GetManipulationOffset().y);

	CWnd* pWnd = GetDlgItem(HEIGHTTEXT);
	pWnd->SetWindowText(t);

	m_toolMain->UpdateTerrainText();

	m_toolMain->windowOpen = true;
}

void TerrainEditorDialogue::OnBnClickedButton40001()
{
	CButton *m_ctlCheck = (CButton*)GetDlgItem(ID_BUTTON40001);
	int ChkBox = m_ctlCheck->GetCheck();
	CString str;

	if (ChkBox == BST_CHECKED)
	{
		m_toolMain->SavePreviousHeightmap();
		m_toolMain->m_terrainTool.SetEnable(true);
		m_toolMain->EnableTerrainText(true);
	}
	else if (ChkBox == BST_UNCHECKED)
	{
		m_toolMain->m_terrainTool.SetEnable(false);
		m_toolMain->EnableTerrainText(false);

		m_TerrainEditorConfirmDialogue.Create(IDD_DIALOG4);	//Start up modeless
		m_TerrainEditorConfirmDialogue.ShowWindow(SW_SHOW);	//show modeless
		m_TerrainEditorConfirmDialogue.m_toolMain = m_toolMain;

		m_toolMain->windowOpen = true;
	}

	m_toolMain->UpdateTerrainText();
	// TODO: Add your control notification handler code here
}

void TerrainEditorDialogue::OnBnClickedButton40002()
{
	CButton *m_ctlCheck = (CButton*)GetDlgItem(ID_BUTTON40002);
	int ChkBox = m_ctlCheck->GetCheck();
	CString str;

	if (ChkBox == BST_CHECKED)
	{
		m_toolMain->m_terrainTool.smoothSculpt = true;
	}
	else if (ChkBox == BST_UNCHECKED)
	{
		m_toolMain->m_terrainTool.smoothSculpt = false;
	}

	m_toolMain->UpdateTerrainText();
}

void TerrainEditorDialogue::OnBnClickedButton40003()
{
	CButton *m_ctlCheck3 = (CButton*)GetDlgItem(ID_BUTTON40003);
	CButton *m_ctlCheck4 = (CButton*)GetDlgItem(ID_BUTTON40004);

	m_ctlCheck3->SetCheck(1);
	m_ctlCheck4->SetCheck(0);

	m_toolMain->m_terrainTool.m_terrainSculptMode = TerrainSculptMode::Hill;
}

void TerrainEditorDialogue::OnBnClickedButton40004()
{
	CButton *m_ctlCheck3 = (CButton*)GetDlgItem(ID_BUTTON40003);
	CButton *m_ctlCheck4 = (CButton*)GetDlgItem(ID_BUTTON40004);

	m_ctlCheck3->SetCheck(0);
	m_ctlCheck4->SetCheck(1);

	m_toolMain->m_terrainTool.m_terrainSculptMode = TerrainSculptMode::Plateau;
}

void TerrainEditorDialogue::OnBnClickedButtonSAVEBUTTON()
{
	m_toolMain->SaveHeightmap();
	m_toolMain->SavePreviousHeightmap();
}

void TerrainEditorDialogue::OnDeltaSpin1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	m_toolMain->m_terrainTool.SetManipulationOffset(DirectX::XMFLOAT3{ m_toolMain->m_terrainTool.GetManipulationOffset().x, m_toolMain->m_terrainTool.GetManipulationOffset().y - pNMUpDown->iDelta, m_toolMain->m_terrainTool.GetManipulationOffset().z });

	CString t;
	t.Format(_T("%d"), (int)m_toolMain->m_terrainTool.GetManipulationOffset().y);

	CWnd* pWnd = GetDlgItem(HEIGHTTEXT);
	pWnd->SetWindowText(t);

	m_toolMain->UpdateTerrainText();
}