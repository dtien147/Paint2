#include "stdafx.h"
#include "Line.h"
#include "resource.h"
#include "MainFrm.h"
#include <string>
#include <fstream>
#include "Database.h"

Line::Line(Point Start, Color color, float Width)
	:Figure(color, Color(0, 0, 0, 0), Width)
{
	m_Start = Start;
	m_End = m_Start;
}

void Line::Draw(Graphics* graphics)
{
	Pen myPen(m_Color, m_Width);
	graphics->DrawLine(&myPen, m_Start, m_End);
}

void Line::SetEndPoint(Point newEnd)
{
	m_End = newEnd;
}

int Line::Kind()
{
	return 0;
}

bool Line::Intersect(Point point)
{
	float m1, m2;
	float d, d1, d2;
	d1 = sqrt((float)((m_Start.X - point.X)*(m_Start.X - point.X) + (point.Y - m_Start.Y)*(point.Y - m_Start.Y)));
	d2 = sqrt((float)((m_End.X - point.X)*(m_End.X - point.X) + (point.Y - m_End.Y)*(point.Y - m_End.Y)));
	d = sqrt((float)((m_End.X - m_Start.X)*(m_End.X - m_Start.X) + (m_Start.Y - m_End.Y)*(m_Start.Y - m_End.Y)));
	if (m_Start.X - m_End.X != 0)
	{
		m1 = (m_Start.Y - m_End.Y) / (float)((m_Start.X - m_End.X));
		m2 = m_Start.Y - m1 * m_Start.X;
		return(abs(point.Y - m1*point.X - m2) < m_Width &&  abs(d - (d1 + d2)) < m_Width/2);
	}
	else
		return(abs(point.X -m_Start.X) < m_Width && (int)d == int(d1 + d2));
}

void Line::Move(int x, int y)
{
	m_Start.X += x;
	m_End.X += x;
	m_Start.Y += y;
	m_End.Y += y;
}

void Line::ViewProperties()
{
	CString str;
	CMainFrame *pMainWnd = CMainFrame::GetCMF();
	pMainWnd->RemoveProperties();
	pMainWnd->AddPropertiesCategory();
	pMainWnd->AddPropertiesPanel(_T("Coordinates"));

	//
	CMFCRibbonEdit* EditBox1 = new CMFCRibbonEdit(ID_INFO1, 50,_T("x1"), -1);
	str.Format(_T("%d"), m_Start.X);
	EditBox1->SetEditText(str);
	pMainWnd->Add(EditBox1, 0);

	CMFCRibbonEdit* EditBox2 = new CMFCRibbonEdit(ID_INFO2, 50,_T("y1"), -1);
	str.Format(_T("%d"), m_Start.Y);
	EditBox2->SetEditText(str);
	pMainWnd->Add(EditBox2, 0);

	CMFCRibbonEdit* EditBox3 = new CMFCRibbonEdit(ID_INFO3, 50,_T("x2"), -1);
	str.Format(_T("%d"), m_End.X);
	EditBox3->SetEditText(str);
	pMainWnd->Add(EditBox3, 0);

	CMFCRibbonEdit* EditBox4 = new CMFCRibbonEdit(ID_INFO4, 50,_T("y2"), -1);
	str.Format(_T("%d"), m_End.Y);
	EditBox4->SetEditText(str);
	pMainWnd->Add(EditBox4, 0);
}

void Line::DrawRect(Graphics* graphics)
{
	Database::Draw(rect(), graphics);
}

void Line::SetStartX(int newX)
{
	m_Start.X = newX;
}

void Line::SetStartY(int newY)
{
	m_Start.Y = newY;
}

void Line::SetEndX(int newX)
{
	m_End.X = newX;
}

void Line::SetEndY(int newY)
{
	m_End.Y = newY;
}

Rect Line::rect()
{
	return Rect(min(m_Start.X, m_End.X) - m_Width/2, min(m_Start.Y, m_End.Y) - m_Width/2, 
		abs(m_Start.X - m_End.X) + m_Width, abs(m_Start.Y - m_End.Y) + m_Width);
}

void Line::Save(ofstream& out)
{
	CString temp;
	temp.Format(_T(" stroke=\"rgb(%d, %d, %d)\""), m_Color.GetRed(), m_Color.GetGreen(), m_Color.GetBlue());
	out << "<line x1=\"" << m_Start.X << "\" y1=\"" << m_Start.Y << "\" x2=\"" << m_End.X << "\" y2=\"" << m_End.Y << "\"";
	out << CT2A(temp);
	out << " stroke-width=\"" << m_Width << "\" />";
}

void Line::Load(string &in)
{
	in.erase(0, in.find_first_of("\"") + 1);
	string temp = in.substr(0, in.find_first_of("\""));
	m_Start.X = stoi(temp);
	in.erase(0, in.find_first_of("\"") + 1);

	in.erase(0, in.find_first_of("\"") + 1);
	temp = in.substr(0, in.find_first_of("\""));
	m_Start.Y = stoi(temp);
	in.erase(0, in.find_first_of("\"") + 1);

	in.erase(0, in.find_first_of("\"") + 1);
	temp = in.substr(0, in.find_first_of("\""));
	m_End.X = stoi(temp);
	in.erase(0, in.find_first_of("\"") + 1);

	in.erase(0, in.find_first_of("\"") + 1);
	temp = in.substr(0, in.find_first_of("\""));
	m_End.Y = stoi(temp);
	in.erase(0, in.find_first_of("\"") + 1);

	int r, g, b;
	in.erase(0, in.find_first_of("(") + 1);
	temp = in.substr(0, in.find_first_of(","));
	r = stoi(temp);
	in.erase(0, in.find_first_of(",") + 2);

	temp = in.substr(0, in.find_first_of(","));
	g = stoi(temp);
	in.erase(0, in.find_first_of(",") + 2);

	temp = in.substr(0, in.find_first_of(","));
	b = stoi(temp);
	m_Color = Color(r, g, b);

	in.erase(0, in.find_first_of("\"") + 1);

	in.erase(0, in.find_first_of("\"") + 1);
	temp = in.substr(0, in.find_first_of("\""));
	m_Width = stof(temp);
	in.erase(0, in.find_first_of("\"") + 1);
}

Figure* Line::Clone()
{
	return new Line(*this);
}

void Line::Move(Point newPos)
{
	Point oldPos = Point(m_Start.X, m_Start.Y);
	Move(newPos.X - oldPos.X, newPos.Y - oldPos.Y);
}

int Line::GetTop()
{
	return rect().Y;
}

int Line::GetLeft()
{
	return rect().X;
}