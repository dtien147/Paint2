#include <iostream>
#include <GdiPlus.h>
using namespace Gdiplus;
using namespace std;


#pragma once
class Figure
{
protected:
	float m_Width;
	Color m_Color;
	Color m_Background_Color;
public:
	Figure(Color PenColor, Color BackgroundColor, float PenWidth);
	virtual void Draw(Graphics* graphics) = 0;
	virtual void SetEndPoint(Point newEnd) = 0;
	virtual bool Intersect(Point point) = 0;
	virtual void Move(int x, int y) = 0;
	virtual void ViewProperties() = 0;
	virtual void DrawRect(Graphics* graphics) = 0;
	void SetWidth(float newWidth);
	void SetColor(Color newColor);
	void SetBackgroundColor(Color color);
	float GetWidth();
	Color GetColor();
	Color GetBackgroundColor();
	virtual void Save(ofstream& out) = 0;
	virtual void Load(string &in) = 0;
	virtual Rect rect()= 0;			
	virtual int Kind() = 0;						// 0 = Line, 1 = Ellipse, 2 = Rectangle
	virtual Figure* Clone() = 0;
	virtual void Move(Point newPos) = 0;
	virtual int GetLeft() = 0;
	virtual int GetTop() = 0;
};