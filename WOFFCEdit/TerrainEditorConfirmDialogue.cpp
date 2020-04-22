#include "TerrainEditorConfirmDialogue.h"
#include <string>
#include "Game.h"
#include "stdafx.h"

IMPLEMENT_DYNAMIC(TerrainEditorConfirmDialogue, CDialogEx)

//Message map.  Just like MFCMAIN.cpp.  This is where we catch button presses etc and point them to a handy dandy method.
BEGIN_MESSAGE_MAP(TerrainEditorConfirmDialogue, CDialogEx)
	ON_COMMAND(IDOK, &TerrainEditorConfirmDialogue::End)					//ok button
	ON_BN_CLICKED(IDOK, &TerrainEditorConfirmDialogue::End)
	ON_BN_CLICKED(IDC_BUTTONYes, &TerrainEditorConfirmDialogue::OnBnClickedButtonYes)
	ON_BN_CLICKED(IDC_BUTTONNO, &TerrainEditorConfirmDialogue::OnBnClickedButtonNo)
END_MESSAGE_MAP()

TerrainEditorConfirmDialogue::TerrainEditorConfirmDialogue(CWnd * pParent)
	: CDialogEx(IDD_DIALOG4, pParent)
{
}

TerrainEditorConfirmDialogue::~TerrainEditorConfirmDialogue()
{
	DestroyWindow();
}

void TerrainEditorConfirmDialogue::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_LIST1, m_listBox);
}

void TerrainEditorConfirmDialogue::End()
{
	m_toolMain->windowOpen = false;

	DestroyWindow();	//destory the window properly.  INcluding the links and pointers created.  THis is so the dialogue can start again. 
}

BOOL TerrainEditorConfirmDialogue::OnInitDialog()
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

void TerrainEditorConfirmDialogue::PostNcDestroy()
{
	m_toolMain->windowOpen = false;

	DestroyWindow();
}

void TerrainEditorConfirmDialogue::OnCancel()
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


void TerrainEditorConfirmDialogue::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}


void TerrainEditorConfirmDialogue::OnBnClickedButtonYes()
{
	m_toolMain->SaveHeightmap();

	End();
}

void TerrainEditorConfirmDialogue::OnBnClickedButtonNo()
{
	m_toolMain->UndoHeightmapChanges();

	End();
}