#include "Figure.h"
#pragma once
class Line: public Figure
{
private:
	Point m_Start;
	Point m_End;
public:
	Line(Point Start, Color color, float Width);
	void Draw(Graphics* graphics);
	void SetEndPoint(Point newEnd);
	int Kind();
	bool Intersect(Point point);
	void Move(int x, int y);
	void ViewProperties();
	void DrawRect(Graphics* graphics);
	void SetStartX(int newX);
	void SetStartY(int newY);
	void SetEndX(int newX);
	void SetEndY(int newY);
	Rect rect();
	void Save(ofstream& out);
	void Load(string &in);
	Figure* Clone();
	void Move(Point newPos);
	int GetLeft();
	int GetTop();
};