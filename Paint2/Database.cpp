#include "stdafx.h"
#include "Database.h"
#include <string>
#include "Line.h"
#include "Rectangle.h"
#include "Ellipse.h"
#include "MainFrm.h"
Color Database::PenColor = BLACK;
float Database::PenWidth = DEFAULT_WIDTH;
Color Database::BackgroundColor = WHITE;
Figure* Database::pFig = NULL;
vector<Figure*> Database::List;
int Database::ToolSelected = -1;
vector<Figure*> Database::SelectedList;
void Database::Draw(Rect rect, Graphics* graph)
{
	Pen myPen(Color(255, 0, 0, 0), 1.0);
	myPen.SetDashStyle(DashStyleDash);
	myPen.SetDashCap(DashCapTriangle);
	graph->DrawRectangle(&myPen, rect);
	SolidBrush Background(Color(25, 0, 0, 255));
	graph->FillRectangle(&Background, rect);
}
void Database::SetColor(COLORREF temp)
{
	PenColor.SetFromCOLORREF(temp);
	Update();
}
void Database::SetWidth(float newWidth)
{
	PenWidth = newWidth;
	Update();
}
void Database::Update()
{
	if(pFig != NULL)
	{
		pFig->SetColor(PenColor);
		pFig->SetWidth(PenWidth);
		pFig->SetBackgroundColor(BackgroundColor);
	}

	if(SelectedList.size() > 0)
		for (unsigned int i = 0; i < SelectedList.size(); i++)
		{
			SelectedList[i]->SetColor(PenColor);
			SelectedList[i]->SetWidth(PenWidth);
			SelectedList[i]->SetBackgroundColor(BackgroundColor);
		}
}
void Database::SetBackGroundColor(COLORREF temp)
{
	BackgroundColor.SetFromCOLORREF(temp);
	Update();
}
Rect Database::ReBalance(int x1, int y1, int x2, int y2)
{
	return Rect(min(x1, x2), min(y1, y2), abs(x1 - x2), abs(y1 - y2));
}
void Database::Default()
{
	for(unsigned int i = 0; i < List.size(); i++)
		delete List[i];
	List.clear();
	pFig = NULL;
	SelectedList.clear();
	PenWidth = 5.0;
	PenColor = BLACK;
	BackgroundColor = WHITE;

}
void Database::Save(CString FilePath)
{
	ofstream out;
	out.open(FilePath);
	out << "<svg xmlns=\"http://www.w3.org/2000/svg\"	 version=\"1.1\"> " << endl;
	for(unsigned int i = 0; i < List.size(); i++)
	{
		List[i]->Save(out);
		out << endl;
	}
	out << "</svg>" ;
	out.close();
}
void Database::Load(CString FilePath)
{
	Default();
	ifstream in;
	in.open(FilePath);
	string temp;
	getline(in, temp);
	while(! in.eof())
	{
		Figure* newFig;
		getline(in, temp);
		string sub = temp.substr(1, temp.find_first_of(" ") - 1);
		if(sub.compare("line") == 0)
			newFig = new Line(Point(NULL, NULL), NULL, NULL);
		else
			if(sub.compare("ellipse") == 0)
				newFig = new Ellip(NULL, NULL, NULL, NULL, NULL);
			else
				if(sub.compare("rect") == 0)
					newFig = new Rectan(Point(NULL, NULL), NULL, NULL, NULL);
				else
					break;	
		newFig->Load(temp);
		List.push_back(newFig);
	}
	in.close();
}
void Database::UpdateData()
{
	PenWidth = pFig->GetWidth();
	PenColor = pFig->GetColor();
	BackgroundColor = pFig->GetBackgroundColor();
}
void Database::Delete(Figure* Fig)
{
	if(Fig == pFig)
		pFig = NULL;
	for(int i = SelectedList.size() - 1; i >= 0; i--)
		if(SelectedList[i] == Fig)
			SelectedList.erase(SelectedList.begin() + i);
	for (int i = List.size() - 1; i >= 0; i--)
		if(Fig == List[i])
		{
			delete List[i];
			List.erase(List.begin()+i);
		}
	CMainFrame *pMainWnd = CMainFrame::GetCMF();
	pMainWnd->UpdateGroup(Fig);
}