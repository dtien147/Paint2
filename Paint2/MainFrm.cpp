
#include "stdafx.h"
#include "Paint2.h"
#include "MainFrm.h"
#include <stdlib.h>
#include <iostream>
#include <string>
#include "Database.h"
#include "Line.h"
#include "Ellipse.h"
#include "Rectangle.h"
#ifdef _DEBUG
#define new DEBUG_NEW

#endif
#define MAX_CFileDialog_FILE_COUNT 99
#define FILE_LIST_BUFFER_SIZE ((MAX_CFileDialog_FILE_COUNT * (MAX_PATH + 1)) + 1)

// CMainFrame

vector<Figure*> Group1, Group2, Group3;
CString FullPath;
IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_FILE_SAVE1, &CMainFrame::OnSave)
	ON_COMMAND(ID_FILE_SAVE_AS1, &CMainFrame::OnSaveAs)
	ON_COMMAND(IDC_TOOLS, &CMainFrame::OnSelectTools)
	ON_COMMAND(ID_PEN_COLOR, &CMainFrame::OnSelectPenColor)
	ON_COMMAND(ID_INFO1, &CMainFrame::OnInfo1)
	ON_COMMAND(ID_INFO2, &CMainFrame::OnInfo2)
	ON_COMMAND(ID_INFO3, &CMainFrame::OnInfo3)
	ON_COMMAND(ID_INFO4, &CMainFrame::OnInfo4)
	ON_COMMAND(ID_WIDTH, &CMainFrame::OnSetWidth)
	ON_COMMAND(ID_BACKGROUND_COLOR, &CMainFrame::OnBackgroundColor)
	ON_COMMAND(IDC_ADD_1, &CMainFrame::OnAdd1)
	ON_COMMAND(IDC_GROUP_1, &CMainFrame::OnGroup1)
	ON_COMMAND(ID_CLEAR_1, &CMainFrame::OnClear1)
	ON_COMMAND(IDC_ADD_2, &CMainFrame::OnAdd2)
	ON_COMMAND(IDC_GROUP_2, &CMainFrame::OnGroup2)
	ON_COMMAND(ID_CLEAR_2, &CMainFrame::OnClear2)
	ON_COMMAND(IDC_ADD_3, &CMainFrame::OnAdd3)
	ON_COMMAND(IDC_GROUP_3, &CMainFrame::OnGroup3)
	ON_COMMAND(ID_CLEAR_3, &CMainFrame::OnClear3)
	ON_COMMAND(ID_FILE_NEW, &CMainFrame::OnNew)
	ON_COMMAND(ID_FILE_OPEN1, &CMainFrame::OnOpen)
END_MESSAGE_MAP()

// CMainFrame construction/destruction
CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{
	Database::Default();
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// set the visual manager used to draw all user interface elements
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));

	// set the visual style to be used the by the visual manager
	CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);

	m_wndRibbonBar.Create(this);
	m_wndRibbonBar.LoadFromResource(IDR_RIBBON);

	// enable Visual Studio 2005 style docking window behavior
	CDockingManager::SetDockingMode(DT_SMART);
	// enable Visual Studio 2005 style docking window auto-hide behavior
	EnableAutoHidePanes(CBRS_ALIGN_ANY);
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG
// CMainFrame message handlers
void CMainFrame::OnSelectTools()
{
	// TODO: Add your command handler code here
	CMFCRibbonGallery* pGallery = DYNAMIC_DOWNCAST(CMFCRibbonGallery, m_wndRibbonBar.FindByID(IDC_TOOLS));
	Database::pFig = NULL;
	Database::SelectedList.clear();
	Database::ToolSelected = pGallery->GetSelectedItem();
	CMainFrame::GetCMF()->RemoveProperties();
}
void CMainFrame::Add(CMFCRibbonEdit* EditBox, int iIndex)
{
	CMFCRibbonCategory* pCategory = m_wndRibbonBar.GetCategory(PROPERTIESINDEX);
	CMFCRibbonPanel* pPanel = pCategory->GetPanel(iIndex);
	pPanel->Add(EditBox);
	m_wndRibbonBar.ForceRecalcLayout();
}
void CMainFrame::Add(CMFCRibbonColorButton* ColorButton, int iIndex)
{
	CMFCRibbonCategory* pCategory = m_wndRibbonBar.GetCategory(PROPERTIESINDEX);
	CMFCRibbonPanel* pPanel = pCategory->GetPanel(iIndex);
	pPanel->Add(ColorButton);
	m_wndRibbonBar.ForceRecalcLayout();
}
void CMainFrame::Add(CMFCRibbonButton* Button, int iIndex)
{
	CMFCRibbonCategory* pCategory = m_wndRibbonBar.GetCategory(PROPERTIESINDEX);
	CMFCRibbonPanel* pPanel = pCategory->GetPanel(iIndex);
	pPanel->Add(Button);
	m_wndRibbonBar.ForceRecalcLayout();
}
void CMainFrame::AddPropertiesCategory()
{
	CMFCRibbonCategory* pCategory = m_wndRibbonBar.AddCategory(_T("Properties"), 0, 0);
}
void CMainFrame::RemoveProperties()
{
	m_wndRibbonBar.RemoveCategory(PROPERTIESINDEX);
	m_wndRibbonBar.ForceRecalcLayout();
}
void CMainFrame::AddPropertiesPanel(CString PanelName)
{
	CMFCRibbonCategory* pCategory = m_wndRibbonBar.GetCategory(PROPERTIESINDEX);
	pCategory->AddPanel(PanelName);
}
void CMainFrame::OnSelectPenColor()
{
	CMFCRibbonColorButton* pColor = DYNAMIC_DOWNCAST(CMFCRibbonColorButton, m_wndRibbonBar.FindByID(ID_PEN_COLOR));
	Database::SetColor(pColor->GetColor());
	this->Invalidate();
}
void CMainFrame::OnInfo1()
{
	CMFCRibbonEdit* pEdit = DYNAMIC_DOWNCAST(CMFCRibbonEdit, m_wndRibbonBar.FindByID(ID_INFO1));
	int temp = _ttoi(pEdit->GetEditText());
	switch(Database::pFig->Kind())
	{
	case 0:
		dynamic_cast<Line*>(Database::pFig)->SetStartX(temp);
		break;
	case 1:
		dynamic_cast<Ellip*>(Database::pFig)->SetCenterX(temp);
		break;
	case 2:
		dynamic_cast<Rectan*>(Database::pFig)->SetX1(temp);
		break;
	}
	Invalidate();
}
void CMainFrame::OnInfo2()
{
	CMFCRibbonEdit* pEdit = DYNAMIC_DOWNCAST(CMFCRibbonEdit, m_wndRibbonBar.FindByID(ID_INFO2));
	int temp = _ttoi(pEdit->GetEditText());
	switch(Database::pFig->Kind())
	{
	case 0:
		dynamic_cast<Line*>(Database::pFig)->SetStartY(temp);
		break;
	case 1:
		dynamic_cast<Ellip*>(Database::pFig)->SetCenterY(temp);
		break;
	case 2:
		dynamic_cast<Rectan*>(Database::pFig)->SetY1(temp);
		break;
	}
	Invalidate();
}
void CMainFrame::OnInfo3()
{
	CMFCRibbonEdit* pEdit = DYNAMIC_DOWNCAST(CMFCRibbonEdit, m_wndRibbonBar.FindByID(ID_INFO3));
	float temp = (float)_ttof(pEdit->GetEditText());
	switch(Database::pFig->Kind())
	{
	case 0:
		dynamic_cast<Line*>(Database::pFig)->SetEndX((int)temp);
		break;
	case 1:
		dynamic_cast<Ellip*>(Database::pFig)->SetRx(temp);
		break;
	case 2:
		dynamic_cast<Rectan*>(Database::pFig)->SetX2((int)temp);
		break;
	}
	Invalidate();
}
void CMainFrame::OnInfo4()
{
	CMFCRibbonEdit* pEdit = DYNAMIC_DOWNCAST(CMFCRibbonEdit, m_wndRibbonBar.FindByID(ID_INFO4));
	float temp = (float)_ttof(pEdit->GetEditText());
	switch(Database::pFig->Kind())
	{
	case 0:
		dynamic_cast<Line*>(Database::pFig)->SetEndY((int)temp);
		break;
	case 1:
		dynamic_cast<Ellip*>(Database::pFig)->SetRy(temp);
		break;
	case 2:
		dynamic_cast<Rectan*>(Database::pFig)->SetY2((int)temp);
		break;
	}
	Invalidate();
}
void CMainFrame::OnSetWidth()
{
	CMFCRibbonEdit* pEdit = DYNAMIC_DOWNCAST(CMFCRibbonEdit, m_wndRibbonBar.FindByID(ID_WIDTH));
	float width = (float)_ttof(pEdit->GetEditText());
	Database::SetWidth(width);
	Invalidate();
}
void CMainFrame::OnBackgroundColor()
{
	CMFCRibbonColorButton* pColor = DYNAMIC_DOWNCAST(CMFCRibbonColorButton, m_wndRibbonBar.FindByID(ID_BACKGROUND_COLOR));
	Database::SetBackGroundColor(pColor->GetColor());
	Invalidate();
}
void CMainFrame::OnAdd1()
{
	int s = 0;
	if(Database::pFig != NULL)
		Group1.push_back(Database::pFig);
	if(Database::SelectedList.size() > 0)
		for(unsigned int i = 0; i < Database::SelectedList.size(); i++)
			Group1.push_back(Database::SelectedList[i]);
}
void CMainFrame::OnGroup1()
{
	Database::SelectedList = Group1;
	Database::pFig = NULL;
	Invalidate();
}
void CMainFrame::OnClear1()
{
	Group1.clear();
}
void CMainFrame::OnAdd2()
{
	int s = 0;
	if(Database::pFig != NULL)
		Group2.push_back(Database::pFig);
	if(Database::SelectedList.size() > 0)
		for(unsigned int i = 0; i < Database::SelectedList.size(); i++)
			Group2.push_back(Database::SelectedList[i]);
}
void CMainFrame::OnGroup2()
{
	Database::SelectedList = Group2;
	Database::pFig = NULL;
	Invalidate();
}
void CMainFrame::OnClear2()
{
	Group2.clear();
}
void CMainFrame::OnAdd3()
{
	int s = 0;
	if(Database::pFig != NULL)
		Group3.push_back(Database::pFig);
	if(Database::SelectedList.size() > 0)
		for(unsigned int i = 0; i < Database::SelectedList.size(); i++)
			Group3.push_back(Database::SelectedList[i]);
}
void CMainFrame::OnGroup3()
{
	Database::SelectedList = Group3;
	Database::pFig = NULL;
	Invalidate();
}
void CMainFrame::OnClear3()
{
	Group3.clear();
}
void CMainFrame::OnNew()
{
	Database::Default();
	Group1.clear();
	Group2.clear();
	Group3.clear();
	FullPath = "";
	UpdateCategory();
}
void CMainFrame::OnSave()
{
	if(FullPath.Compare(_T("")) == 0)
		OnSaveAs();
	else
		Database::Save(FullPath);
}
void CMainFrame::OnSaveAs()
{
	char strFilter[] = { "SVG (*.svg)|*.svg|" }; 
	CFileDialog FileDlg(FALSE, CString(".svg"), NULL, 0, CString(strFilter)); 
	if(FileDlg.DoModal() == IDOK)
	{
		CString FileName = FileDlg.GetFileName(); //filename
		CString FolderPath = FileDlg.GetFolderPath(); //filepath (folders)
		FullPath = FolderPath + _T("\\") + FileName;
		Database::Save(FullPath);
	}
}
void CMainFrame::OnOpen()
{
	char strFilter[] = { "SVG (*.svg)|*.svg|" }; 
	CFileDialog FileDlg(TRUE, CString(".svg"), NULL, 0, CString(strFilter));
	if(FileDlg.DoModal() == IDOK)
	{
		OnNew();
		CString FileName = FileDlg.GetFileName(); //filename
		CString FolderPath = FileDlg.GetFolderPath(); //filepath (folders)
		CString FullPath = FolderPath + _T("\\") + FileName;
		Database::Load(FullPath);
		Invalidate();
	}
}
void CMainFrame::UpdateGroup(Figure* pFig)
{
	for(int i = Group1.size() - 1; i >= 0; i--)
		if(Group1[i] == pFig)
			Group1.erase(Group1.begin() + i); 
	for(int i = Group2.size() - 1; i >= 0; i--)
		if(Group2[i] == pFig)
			Group2.erase(Group1.begin() + i); 
	for(int i = Group3.size() - 1; i >= 0; i--)
		if(Group3[i] == pFig)
			Group3.erase(Group3.begin() + i); 
}
void CMainFrame::UpdateCategory()
{
	CString temp;
	CMFCRibbonEdit* pEdit = DYNAMIC_DOWNCAST (CMFCRibbonEdit, m_wndRibbonBar.FindByID(ID_WIDTH));
	temp.Format(_T("%.1f"), Database::PenWidth);
	pEdit->SetEditText(temp);
	CMFCRibbonColorButton* pColor = DYNAMIC_DOWNCAST(CMFCRibbonColorButton, m_wndRibbonBar.FindByID(ID_PEN_COLOR));
	pColor->SetColor(Database::PenColor.ToCOLORREF());
	pColor = DYNAMIC_DOWNCAST(CMFCRibbonColorButton, m_wndRibbonBar.FindByID(ID_BACKGROUND_COLOR));
	pColor->SetColor(Database::BackgroundColor.ToCOLORREF());
	Invalidate();
}