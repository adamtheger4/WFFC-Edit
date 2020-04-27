#include "TerrainEditorPaintDialogue.h"
#include <string>
#include "stdafx.h"

IMPLEMENT_DYNAMIC(TerrainEditorPaintDialogue, CDialogEx)

//Message map.  Just like MFCMAIN.cpp.  This is where we catch button presses etc and point them to a handy dandy method.
BEGIN_MESSAGE_MAP(TerrainEditorPaintDialogue, CDialogEx)
	ON_COMMAND(IDOK, &TerrainEditorPaintDialogue::End)					//ok button
	ON_BN_CLICKED(IDOK, &TerrainEditorPaintDialogue::End)
	ON_NOTIFY(UDN_DELTAPOS, IDC_HEIGHTSPIN, &TerrainEditorPaintDialogue::OnDeltaSpin1)
	ON_BN_CLICKED(ID_BUTTON40001, &TerrainEditorPaintDialogue::OnBnClickedButton40001)
	ON_BN_CLICKED(SAVEBUTTON, &TerrainEditorPaintDialogue::OnBnClickedButtonSAVEBUTTON)
	ON_LBN_SELCHANGE(IDC_TEXLIST, &TerrainEditorPaintDialogue::OnLbnSelchangeTexlist)
END_MESSAGE_MAP()

TerrainEditorPaintDialogue::TerrainEditorPaintDialogue(CWnd * pParent, ToolMain in_toolSystem)
	: CDialogEx(IDD_DIALOG3, pParent)
{
	m_toolMain = &in_toolSystem;
}


TerrainEditorPaintDialogue::TerrainEditorPaintDialogue(CWnd * pParent)
	: CDialogEx(IDD_DIALOG3, pParent)
{
}

TerrainEditorPaintDialogue::~TerrainEditorPaintDialogue()
{
	DestroyWindow();
}

void TerrainEditorPaintDialogue::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TEXLIST, m_listBox);
}

void TerrainEditorPaintDialogue::End()
{
	m_toolMain->windowOpen = false;

	DestroyWindow();	//destory the window properly.  INcluding the links and pointers created.  THis is so the dialogue can start again. 
}

void TerrainEditorPaintDialogue::Select()
{
	int index = m_listBox.GetCurSel();
	CString currentSelectionValue;

	m_listBox.GetText(index, currentSelectionValue);

	m_currentSelection = _ttoi(currentSelectionValue);
}

BOOL TerrainEditorPaintDialogue::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void TerrainEditorPaintDialogue::PostNcDestroy()
{
	m_toolMain->windowOpen = false;

	DestroyWindow();
}

void TerrainEditorPaintDialogue::OnCancel()
{
	m_toolMain->windowOpen = false;

	DestroyWindow();
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


void TerrainEditorPaintDialogue::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}

void TerrainEditorPaintDialogue::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult)
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

void TerrainEditorPaintDialogue::EnableTerrainManip(NMHDR * pNMHDR, LRESULT * pResult)
{

}


void TerrainEditorPaintDialogue::SetText()
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


void TerrainEditorPaintDialogue::SetData()
{
	CButton *m_ctlCheck1 = (CButton*)GetDlgItem(ID_BUTTON40001);
	if (m_toolMain->m_terrainTool.GetEnable() && m_toolMain->m_terrainTool.m_terrainSculptMode == TerrainSculptMode::Paint)
	{
		m_ctlCheck1->SetCheck(1);
	}
	else
	{
		m_ctlCheck1->SetCheck(0);
	}

	//roll through all the textures and put an entry for each in the listbox
	int numTextures = m_toolMain->m_terrainTool.m_terrainTexturePaths.GetSize();

	for (int i = 0; i < numTextures; i++)
	{
		m_listBox.AddString(m_toolMain->m_terrainTool.m_terrainTexturePaths.GetAt(i));
	}

	m_toolMain->windowOpen = true;
}

void TerrainEditorPaintDialogue::OnBnClickedButton40001()
{
	CButton *m_ctlCheck = (CButton*)GetDlgItem(ID_BUTTON40001);
	int ChkBox = m_ctlCheck->GetCheck();
	CString str;

	if (ChkBox == BST_CHECKED)
	{
		m_toolMain->SavePreviousTerrainTextures();
		m_toolMain->m_terrainTool.m_terrainSculptMode = TerrainSculptMode::Paint;
		m_toolMain->m_terrainTool.SetEnable(true);
		m_toolMain->EnableTerrainText(true);

	}
	else if (ChkBox == BST_UNCHECKED)
	{
		m_toolMain->m_terrainTool.SetEnable(false);
		m_toolMain->EnableTerrainText(false);

		m_TerrainEditorConfirmDialogue.Create(IDD_DIALOG4);	//Start up modeless
		m_TerrainEditorConfirmDialogue.ShowWindow(SW_SHOW);	//show modeless
		m_TerrainEditorConfirmDialogue.m_terrainMode = TerrainSculptMode::Paint;
		m_TerrainEditorConfirmDialogue.m_toolMain = m_toolMain;

		m_toolMain->windowOpen = true;
	}

	m_toolMain->UpdateTerrainText();
	// TODO: Add your control notification handler code here
}

void TerrainEditorPaintDialogue::OnBnClickedButtonSAVEBUTTON()
{
	if (m_toolMain->m_terrainTool.m_terrainSculptMode == TerrainSculptMode::Paint)
	{
		m_toolMain->SaveTerrainTextures();
		m_toolMain->SavePreviousTerrainTextures();
	}
}

void TerrainEditorPaintDialogue::OnDeltaSpin1(NMHDR *pNMHDR, LRESULT *pResult)
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

void TerrainEditorPaintDialogue::OnLbnSelchangeTexlist()
{
	int index = m_listBox.GetCurSel();

	CString currentSelectionValue;

	m_listBox.GetText(index, currentSelectionValue);

	//Append the directory to the selected file.
	CString cs = "database/data/terraintextures/" + currentSelectionValue;

	// Convert a TCHAR string to a LPCSTR
	CT2CA ConvertedAnsiString(cs);

	// construct a std::string using the LPCSTR input
	std::string s(ConvertedAnsiString);

	m_toolMain->m_terrainTool.m_paintTexturePath = s;
	m_toolMain->LoadTextureToPaint(s);
}
