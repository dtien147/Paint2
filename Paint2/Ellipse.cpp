#include "stdafx.h"
#include "resource.h"
#include "Ellipse.h"
#include "MainFrm.h"
#include <string>
#include <fstream>
#include "Database.h"

Ellip::Ellip(int cx, int cy, Color PenColor, Color BackgroundColor, float PenWidth)
	:Figure(PenColor, BackgroundColor, PenWidth)
{
	m_cx = cx;
	m_cy = cy;
}

void Ellip::Draw(Graphics* graphics)
{
	Pen myPen(m_Color, m_Width);
	graphics->DrawEllipse(&myPen, m_cx - m_rx, m_cy - m_ry, 2 * m_rx, 2 * m_ry);
	SolidBrush myBrush(m_Background_Color);
	graphics->FillEllipse(&myBrush, (REAL)(m_cx - m_rx), (REAL)(m_cy - m_ry), (REAL)(2 * m_rx), (REAL)(2 * m_ry));
}

void Ellip::SetEndPoint(Point newEnd)
{
	m_rx = (float)newEnd.X - m_cx;
	m_ry = (float)newEnd.Y - m_cy;
}

bool Ellip::Intersect(Point point)
{
	float rx2 = (m_rx + m_Width/2)*(m_rx + m_Width/2);
	float ry2 = (m_ry + m_Width/2)*(m_ry + m_Width/2);
	int x2 = (point.X - m_cx) * (point.X - m_cx);
	int y2 = (point.Y - m_cy) * (point.Y - m_cy);
	//(x - m_cx)^2/a^2 + (y - m_cy)^2/b^2 = 1
	//or (x - m_cy)^2/a^2 + (y - m_cx)^2/b^2 = 1
	//a is m_rx (a is half of length); b is m_ry (b is halft of heigth)
	if(x2/rx2 + y2/ry2 <=1)
		return 1;
	return 0;
}

void Ellip::Move(int x, int y)
{
	m_cx += x;
	m_cy += y;
}

void Ellip::DrawRect(Graphics* graphics)
{
	Database::Draw(rect(), graphics);
}

Rect Ellip::rect()
{
	return Rect(m_cx - (int)(abs(m_rx) + m_Width/2), 
		m_cy - (int)(abs(m_ry) + m_Width/2), 
		(int)(abs(m_rx) * 2 + m_Width), (int)(abs(m_ry) * 2 + m_Width));
}

int Ellip::Kind() 
{
	return 1;
}

void Ellip::SetCenterX(int cx)
{
	m_cx = cx;
}

void Ellip::SetCenterY(int cy)
{
	m_cy = cy;
}

void Ellip::SetRx(float rx)
{
	m_rx = rx;
}

void Ellip::SetRy(float ry)
{
	m_ry = ry;
}

void Ellip::ViewProperties()
{
	CString str;
	CMainFrame *pMainWnd = CMainFrame::GetCMF();
	pMainWnd->RemoveProperties();
	pMainWnd->AddPropertiesCategory();
	pMainWnd->AddPropertiesPanel(_T("Coordinates"));

	//
	CMFCRibbonEdit* EditBox1 = new CMFCRibbonEdit(ID_INFO1, 50,_T("cx"), -1);
	str.Format(_T("%d"), m_cx);
	EditBox1->SetEditText(str);
	pMainWnd->Add(EditBox1, 0);

	CMFCRibbonEdit* EditBox2 = new CMFCRibbonEdit(ID_INFO2, 50,_T("cy"), -1);
	str.Format(_T("%d"), m_cy);
	EditBox2->SetEditText(str);
	pMainWnd->Add(EditBox2, 0);

	CMFCRibbonEdit* EditBox3 = new CMFCRibbonEdit(ID_INFO3, 50,_T("rx"), -1);
	str.Format(_T("%f"), m_rx);
	EditBox3->SetEditText(str);
	pMainWnd->Add(EditBox3, 0);

	CMFCRibbonEdit* EditBox4 = new CMFCRibbonEdit(ID_INFO4, 50,_T("ry"), -1);
	str.Format(_T("%f"), m_ry);
	EditBox4->SetEditText(str);
	pMainWnd->Add(EditBox4, 0);
}

void Ellip::Save(ofstream& out)
{
	CString temp;
	temp.Format(_T(" fill=\"rgb(%d, %d, %d)\""), m_Background_Color.GetRed(), m_Background_Color.GetGreen(), m_Background_Color.GetBlue());
	out << "<ellipse cx=\"" << m_cx << "\" cy=\"" << m_cy << "\" rx=\"" << m_rx << "\" ry=\"" << m_ry << "\"";
	out << CT2A(temp);
	temp.Format(_T(" stroke=\"rgb(%d, %d, %d)\""), m_Color.GetRed(), m_Color.GetGreen(), m_Color.GetBlue());
	out << CT2A(temp);
	out << " stroke-width=\"" << m_Width << "\" />";
}

void Ellip::Load(string &in)
{
	in.erase(0, in.find_first_of("\"") + 1);
	string temp = in.substr(0, in.find_first_of("\""));
	m_cx = stoi(temp);
	in.erase(0, in.find_first_of("\"") + 1);

	in.erase(0, in.find_first_of("\"") + 1);
	temp = in.substr(0, in.find_first_of("\""));
	m_cy = stoi(temp);
	in.erase(0, in.find_first_of("\"") + 1);

	in.erase(0, in.find_first_of("\"") + 1);
	temp = in.substr(0, in.find_first_of("\""));
	m_rx = stof(temp);
	in.erase(0, in.find_first_of("\"") + 1);

	in.erase(0, in.find_first_of("\"") + 1);
	temp = in.substr(0, in.find_first_of("\""));
	m_ry = stof(temp);
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

Figure* Ellip::Clone()
{
	return new Ellip(*this);
}

void Ellip::Move(Point newPos)
{
	Point oldPos = Point(m_cx, m_cy);
	Move(newPos.X - oldPos.X, newPos.Y - oldPos.Y);
}

int Ellip::GetLeft()
{
	return rect().X;
}

int Ellip::GetTop()
{
	return rect().Y;
}