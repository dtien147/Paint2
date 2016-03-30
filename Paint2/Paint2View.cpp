
#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Paint2.h"
#endif

#include "Paint2Doc.h"
#include "Paint2View.h"
#include <GdiPlus.h>
#include "Line.h"
#include "MainFrm.h"
#include "Database.h"
#include "Ellipse.h"
#include "Rectangle.h"
bool isDraw = false;
bool isMove = false;
bool isSelect = false;
bool isMouseDown = false;

// CPaint2View

IMPLEMENT_DYNCREATE(CPaint2View, CView)

BEGIN_MESSAGE_MAP(CPaint2View, CView)
	// Standard printing commands
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_EDIT_CUT, &CPaint2View::OnEditCut)
	ON_COMMAND(ID_EDIT_COPY, &CPaint2View::OnEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, &CPaint2View::OnEditPaste)
//	ON_WM_HSCROLL()
//	ON_WM_VSCROLL()
END_MESSAGE_MAP()

// CPaint2View construction/destruction
ULONG_PTR m_gdiplusToken;
CPaint2View::CPaint2View()
{
	// TODO: add construction code here
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);
}

CPaint2View::~CPaint2View()
{
}

BOOL CPaint2View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CPaint2View drawing

void CPaint2View::OnDraw(CDC* /*pDC*/)
{
	CPaint2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}

CPaint2View * CPaint2View::GetView()
{
	CMDIChildWnd * pChild =
		((CMDIFrameWnd*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();

	if ( !pChild )
		return NULL;

	CView * pView = pChild->GetActiveView();

	if ( !pView )
		return NULL;

	// Fail if view is of wrong kind
	if ( ! pView->IsKindOf( RUNTIME_CLASS(CPaint2View) ) )
		return NULL;

	return (CPaint2View *) pView;
}

CPoint RightPos;
void CPaint2View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	RightPos = point;
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CPaint2View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CPaint2View diagnostics

#ifdef _DEBUG
void CPaint2View::AssertValid() const
{
	CView::AssertValid();
}

void CPaint2View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CPaint2Doc* CPaint2View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPaint2Doc)));
	return (CPaint2Doc*)m_pDocument;
}
#endif //_DEBUG


// CPaint2View message handlers

Rect SelectRect;
bool isMultiSelect;
void CPaint2View::OnPaint()
{
	CRect rect;
	CPaintDC dc(this);
	Graphics graphics(dc.m_hDC); 
	GetClientRect(&rect);
	Rect rc(rect.left,rect.top,rect.right,rect.bottom);
	Bitmap bmp(rect.right,rect.bottom);
	// Create a Graphics object that is associated with the image.
	Graphics* graph = Graphics::FromImage(&bmp);
	SolidBrush brush(Color(255, 255, 255, 255));
	graph->FillRectangle(&brush, rc);
	graph->SetSmoothingMode(SmoothingModeAntiAlias);
	for(unsigned int i = 0; i < Database::List.size(); i++)
		Database::List[i]->Draw(graph);
	if(Database::pFig != NULL)
		Database::pFig->DrawRect(graph);
	if(Database::SelectedList.size() > 0)
		for(unsigned int i = 0; i < Database::SelectedList.size(); i++)
			Database::SelectedList[i]->DrawRect(graph);
	if(isMultiSelect)
		Database::Draw(SelectRect, graph);
	graphics.DrawImage(&bmp,rect.left,rect.top,rect.right,rect.bottom);
	delete graph;
}

CPoint oldLocation, newLocation;
void CPaint2View::OnLButtonDown(UINT nFlags, CPoint point)
{
	Database::pFig = NULL;
	CMainFrame *pMainWnd = CMainFrame::GetCMF();
	pMainWnd->RemoveProperties();
	isMove = false;
	switch(Database::ToolSelected)
	{
	case 1:
		Database::pFig = new Line(Point(point.x, point.y), Database::PenColor, Database::PenWidth);
		Database::List.push_back(Database::pFig);
		isMouseDown = true;
		break;
	case 2:
		Database::pFig = new Ellip(point.x, point.y, Database::PenColor, Database::BackgroundColor, Database::PenWidth);
		Database::List.push_back(Database::pFig);
		isMouseDown = true;
		break;
	case 0:
		oldLocation = point;
		isMove = false;
		for(unsigned int i = 0; i < Database::SelectedList.size(); i++)
			if(Database::SelectedList[i]->Intersect(Point(point.x, point.y)))
			{
				isMove = true;
				oldLocation = point;
			}
		if(!isMove)
		{
			Database::SelectedList.clear();
			SelectRect.X = point.x;
			SelectRect.Y = point.y;
			isMultiSelect = true;
		}
		isMouseDown = true;
		break;
	case 4:
		Database::pFig = new Rectan(Point(point.x, point.y), Database::PenColor, Database::BackgroundColor, Database::PenWidth);
		Database::List.push_back(Database::pFig);
		isMouseDown = true;
		break;
	default:
		for(unsigned int i = 0; i < Database::List.size(); i++)
			if(Database::List[i]->Intersect(Point(point.x, point.y)))
			{
				oldLocation = point;
				Database::pFig = Database::List[i];
				isMouseDown = true;
				isMove = true;
			}
	}
	CView::OnLButtonDown(nFlags, point);
}

void CPaint2View::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if(isMultiSelect)
	{
		for(unsigned int i = 0; i < Database::List.size(); i++)
			if(SelectRect.IntersectsWith(Database::List[i]->rect()))
				Database::SelectedList.push_back(Database::List[i]);
		isMultiSelect = false;
		Invalidate();
	}
	if (isMove && Database::pFig != NULL)
	{
		Database::pFig->ViewProperties();
		CMainFrame *pMainWnd = CMainFrame::GetCMF();
		Database::UpdateData();
		pMainWnd->UpdateCategory();
	}
	isMouseDown = false;
	isMove = false;
	CView::OnLButtonUp(nFlags, point);
}


void CPaint2View::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if(isMouseDown)
	{
		switch (Database::ToolSelected)
		{
		case 0:
			if(isMultiSelect)
			{
				newLocation = point;
				SelectRect = Database::ReBalance(oldLocation.x, oldLocation.y, newLocation.x, newLocation.y);
			}
			else
			{
				newLocation = point;
				for(unsigned int i = 0; i < Database::SelectedList.size(); i++)
					Database::SelectedList[i]->Move(newLocation.x - oldLocation.x, newLocation.y - oldLocation.y);
				oldLocation = newLocation;
			}
			Invalidate();
			break;
		case 1:
		case 2:
		case 4:
			Database::pFig->SetEndPoint(Point(point.x, point.y));
			this->Invalidate(TRUE);
			break;
		default:
			newLocation = point;
			Database::pFig->Move(newLocation.x - oldLocation.x, newLocation.y - oldLocation.y);
			Invalidate();
			oldLocation = newLocation;
		}	
	}
	CView::OnMouseMove(nFlags, point);
}


BOOL CPaint2View::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return 1;
}


vector<Figure*> Cut;
vector<Figure*> Copy;
void CPaint2View::OnEditCut()
{
	for (unsigned int i = 0; i < Cut.size(); i++)
		delete Cut[i];
	Cut.clear();

	for (unsigned int i = 0; i < Cut.size(); i++)
		delete Copy[i];
	Copy.clear();

	if(Database::pFig != NULL)
	{
		Cut.push_back(Database::pFig->Clone());
		Database::Delete(Database::pFig);
		Database::pFig = NULL;
	}

	if(Database::SelectedList.size() > 0)
	{
		for(unsigned int i = 0; i < Database::SelectedList.size(); i++)
			Cut.push_back(Database::SelectedList[i]->Clone());	
		for(int i = Database::SelectedList.size() - 1; i >= 0; i--)
			Database::Delete(Database::SelectedList[i]);
		Database::SelectedList.clear();
	}

	this->Invalidate();
}
void CPaint2View::OnEditCopy()
{
	for (unsigned int i = 0; i < Copy.size(); i++)
		delete Copy[i];
	Copy.clear();

	for (unsigned int i = 0; i < Cut.size(); i++)
		delete Cut[i];
	Cut.clear();

	if(Database::pFig != NULL)
		Copy.push_back(Database::pFig->Clone());

	if(Database::SelectedList.size() > 0)
		for(unsigned int i = 0; i < Database::SelectedList.size(); i++)
			Copy.push_back(Database::SelectedList[i]->Clone());

}
void CPaint2View::OnEditPaste()
{
	CPoint CurrentPos;
	if(RightPos.x != 0 && RightPos.y != 0)
		CurrentPos = RightPos;
	else
	{
		POINT p;
		GetCursorPos(&p);
		ScreenToClient(&p);
		CurrentPos = p;
	}

	if(Cut.size() > 0)
	{
		Database::pFig = NULL;
		Database::SelectedList.clear();
		int iLeft = 0;
		for(unsigned int i = 0; i < Cut.size(); i++)
			if(Cut[i]->GetLeft() < Cut[iLeft]->GetLeft())
				iLeft = i;

		int x, y;
		x = CurrentPos.x - Cut[iLeft]->GetLeft();
		y = CurrentPos.y - Cut[iLeft]->GetTop();
		for(unsigned int i = 0; i < Cut.size(); i++)
		{
			Cut[i]->Move(x, y);
			Figure* newFig = Cut[i]->Clone();
			Database::List.push_back(newFig);
			Database::SelectedList.push_back(newFig);
		}
	}

	if(Copy.size() > 0)
	{
		Database::pFig = NULL;
		Database::SelectedList.clear();
		int iLeft = 0;
		for(unsigned int i = 0; i < Copy.size(); i++)
			if(Copy[i]->GetLeft() < Copy[iLeft]->GetLeft())
				iLeft = i;

		int x, y;
		x = CurrentPos.x - Copy[iLeft]->GetLeft();
		y = CurrentPos.y - Copy[iLeft]->GetTop();
		for(unsigned int i = 0; i < Copy.size(); i++)
		{
			Copy[i]->Move(x, y);
			Figure* newFig = Copy[i]->Clone();
			Database::List.push_back(newFig);
			Database::SelectedList.push_back(newFig);
		}
	}
	RightPos = CPoint(0, 0);
	Invalidate();
}