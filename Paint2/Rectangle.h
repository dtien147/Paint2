#include "Figure.h"

class Rectan: public Figure
{
private:
	int m_x1, m_y1, m_x2, m_y2;
public:
	Rectan(Point point, Color PenColor, Color BackgroundColor, float PenWidth);
	void Draw(Graphics* graphics);
	void SetEndPoint(Point newEnd);
	bool Intersect(Point point);
	void Move(int x, int y);
	void ViewProperties();
	void DrawRect(Graphics* graphics);
	Rect rect();
	int Kind();
	void SetX1(int x1);
	void SetX2(int x2);
	void SetY1(int y1);
	void SetY2(int y2);
	void Save(ofstream& out);
	void Load(string &in);
	Figure* Clone();
	void Move(Point newPos);
	int GetLeft();
	int GetTop();
};