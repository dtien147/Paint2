#include "stdafx.h"
#include "resource.h"
#include "Rectangle.h"
#include "MainFrm.h"
#include <string>
#include <fstream>
#include "Database.h"

Rectan::Rectan(Point point, Color PenColor, Color BackgroundColor, float PenWidth)
	:Figure(PenColor, BackgroundColor, PenWidth)
{
	m_x2 = m_x1 = point.X;
	m_y2 = m_y1 = point.Y;
}

void Rectan::Draw(Graphics* graphics)
{
	Pen myPen(m_Color, m_Width);
	graphics->DrawRectangle(&myPen, min(m_x1, m_x2), min(m_y1, m_y2), abs(m_x1 - m_x2), abs(m_y1 - m_y2)); 
	SolidBrush myBrush(m_Background_Color);
	graphics->FillRectangle(&myBrush, min(m_x1, m_x2), min(m_y1, m_y2), abs(m_x1 - m_x2), abs(m_y1 - m_y2));
}

void Rectan::SetEndPoint(Point newEnd)
{
	m_x2 = newEnd.X;
	m_y2 = newEnd.Y;
}

bool Rectan::Intersect(Point point)
{
	float width = abs(m_x1 - m_x2) + m_Width;
	float height = abs(m_y1 - m_y2) + m_Width;

	float x1 = (int)min(m_x1, m_x2) - m_Width/2;
	float x2 = (int)max(m_x1, m_x2) + m_Width/2;
	float y1 = (int)min(m_y1, m_y2) - m_Width/2;
	float y2 = (int)max(m_y1, m_y2) + m_Width/2;

	if(abs((abs(point.Y - y1) + abs(point.Y - y2) - height)) < 0.1
		&& abs((abs(point.X - x1) + abs(point.X - x2) - width)) < 0.1)
		return true;
	return false;
}

void Rectan::Move(int x, int y)
{
	m_x1 += x;
	m_x2 += x;
	m_y1 += y;
	m_y2 += y;
}

void Rectan::ViewProperties()
{
	CString str;
	CMainFrame *pMainWnd = CMainFrame::GetCMF();
	pMainWnd->RemoveProperties();
	pMainWnd->AddPropertiesCategory();
	pMainWnd->AddPropertiesPanel(_T("Coordinates"));

	//
	CMFCRibbonEdit* EditBox1 = new CMFCRibbonEdit(ID_INFO1, 50,_T("x1"), -1);
	str.Format(_T("%d"), m_x1);
	EditBox1->SetEditText(str);
	pMainWnd->Add(EditBox1, 0);

	CMFCRibbonEdit* EditBox2 = new CMFCRibbonEdit(ID_INFO2, 50,_T("y1"), -1);
	str.Format(_T("%d"), m_y1);
	EditBox2->SetEditText(str);
	pMainWnd->Add(EditBox2, 0);

	CMFCRibbonEdit* EditBox3 = new CMFCRibbonEdit(ID_INFO3, 50,_T("x2"), -1);
	str.Format(_T("%d"), m_x2);
	EditBox3->SetEditText(str);
	pMainWnd->Add(EditBox3, 0);

	CMFCRibbonEdit* EditBox4 = new CMFCRibbonEdit(ID_INFO4, 50,_T("y2"), -1);
	str.Format(_T("%d"), m_y2);
	EditBox4->SetEditText(str);
	pMainWnd->Add(EditBox4, 0);
}

void Rectan::DrawRect(Graphics* graphics)
{
	Database::Draw(rect(), graphics);
}

Rect Rectan::rect()
{
	return Rect((int)(min(m_x1, m_x2)-m_Width/2 - 1), (int)(min(m_y1, m_y2)-m_Width/2 - 1), 
		(int)(abs(m_x1 - m_x2)+m_Width+3), (int)(abs(m_y1 - m_y2)+m_Width+3));
}

int Rectan::Kind()
{
	return 2;
}

void Rectan::SetX1(int x1)
{
	m_x1 = x1;
}

void Rectan::SetX2(int x2)
{
	m_x2 = x2;
}

void Rectan::SetY1(int y1)
{
	m_y1 = y1;
}

void Rectan::SetY2(int y2)
{
	m_y2 = y2;
}

void Rectan::Save(ofstream& out)
{
	int x = min(m_x1, m_x2);
	int y = min(m_y1, m_y2);
	int width = abs(m_x1 - m_x2);
	int height = abs(m_y1 - m_y2);
	CString temp;
	temp.Format(_T(" fill=\"rgb(%d, %d, %d)\""), m_Background_Color.GetRed(), m_Background_Color.GetGreen(), m_Background_Color.GetBlue());
	out << "<rect x=\"" << x << "\" y=\"" << y << "\" width=\"" << width << "\" height=\"" << height << "\"";
	out << CT2A(temp);
	temp.Format(_T(" stroke=\"rgb(%d, %d, %d)\""), m_Color.GetRed(), m_Color.GetGreen(), m_Color.GetBlue());
	out << CT2A(temp);
	out << " stroke-width=\"" << m_Width << "\" />";
}

void Rectan::Load(string &in)
{
	in.erase(0, in.find_first_of("\"") + 1);
	string temp = in.substr(0, in.find_first_of("\""));
	m_x1 = stoi(temp);
	in.erase(0, in.find_first_of("\"") + 1);

	in.erase(0, in.find_first_of("\"") + 1);
	temp = in.substr(0, in.find_first_of("\""));
	m_y1 = stoi(temp);
	in.erase(0, in.find_first_of("\"") + 1);

	int width, height;
	in.erase(0, in.find_first_of("\"") + 1);
	temp = in.substr(0, in.find_first_of("\""));
	width = stoi(temp);
	in.erase(0, in.find_first_of("\"") + 1);

	in.erase(0, in.find_first_of("\"") + 1);
	temp = in.substr(0, in.find_first_of("\""));
	height = stoi(temp);
	in.erase(0, in.find_first_of("\"") + 1);

	m_x2 = width + m_x1;
	m_y2 = height + m_y1;

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
	m_Background_Color = Color(r, g, b);
	in.erase(0, in.find_first_of("\"") + 1);

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

Figure* Rectan::Clone()
{
	return new Rectan(*this);
}

void Rectan::Move(Point newPos)
{
	Point oldPos = Point(min(m_x1, m_x2), min(m_y1, m_y2));
	Move(newPos.X - oldPos.X, newPos.Y - oldPos.Y);
}

int Rectan::GetLeft()
{
	return rect().X;
}

int Rectan::GetTop()
{
	return rect().Y;
}