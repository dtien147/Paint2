#include "Figure.h"

#pragma once
class Ellip : public Figure
{
private:
	int m_cx, m_cy;
	float m_rx, m_ry;
public:
	Ellip(int cx, int cy, Color PenColor, Color BackgroundColor, float PenWidth);
	void Draw(Graphics* graphics);
	void SetEndPoint(Point newEnd);
	bool Intersect(Point point);
	void Move(int x, int y);
	void DrawRect(Graphics* graphics);
	Rect rect();
	int Kind();
	void SetCenterX(int cx);
	void SetCenterY(int cy);
	void SetRx(float rx);
	void SetRy(float ry);
	void ViewProperties();
	void Save(ofstream& out);
	void Load(string &in);
	Figure* Clone();
	void Move(Point newPos);
	int GetLeft();
	int GetTop();
};