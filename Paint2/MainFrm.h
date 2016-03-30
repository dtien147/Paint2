#include "Figure.h"

#pragma once
class CMainFrame : public CFrameWndEx
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CMFCRibbonBar     m_wndRibbonBar;
	CMFCRibbonApplicationButton m_MainButton;
	CMFCToolBarImages m_PanelImages;
	CMFCRibbonStatusBar  m_wndStatusBar;

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnSelectTools();
	void AddPropertiesCategory();
	void RemoveProperties();
	void Add(CMFCRibbonEdit* EditBox, int iIndex);
	void Add(CMFCRibbonColorButton* ColorButton, int iIndex);
	void Add(CMFCRibbonButton* Button, int iIndex);
	static CMainFrame* GetCMF()
	{
		return (CMainFrame *)AfxGetMainWnd();
	}

	void AddPropertiesPanel(CString PanelName);

	afx_msg void OnSelectPenColor();
	afx_msg void OnInfo1();
	afx_msg void OnInfo2();
	afx_msg void OnInfo3();
	afx_msg void OnInfo4();
	afx_msg void OnSetWidth();
	afx_msg void OnBackgroundColor();
	afx_msg void OnAdd1();
	afx_msg void OnGroup1();
	afx_msg void OnClear1();
	afx_msg void OnAdd2();
	afx_msg void OnGroup2();
	afx_msg void OnClear2();
	afx_msg void OnAdd3();
	afx_msg void OnGroup3();
	afx_msg void OnClear3();

	afx_msg void OnNew();
	afx_msg void OnSave();
	afx_msg void OnSaveAs();
	afx_msg void OnOpen();

	void UpdateGroup(Figure* pFig);
	void UpdateCategory();
};



