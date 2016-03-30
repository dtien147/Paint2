#include <GdiPlus.h>
#include <vector>
#include <fstream>
#include "Figure.h"
using namespace std;
using namespace Gdiplus;

#pragma once
#define WHITE Color(255, 255, 255)
#define BLACK Color(0, 0, 0)
#define DEFAULT_WIDTH 5.0
#define PROPERTIESINDEX 2
class Database
{
public:
	virtual ~Database() = 0;
	static Color PenColor;
	static Color BackgroundColor;
	static float PenWidth;
	static Figure* pFig;
	static vector<Figure*> List;
	static vector<Figure*> SelectedList;
	static int ToolSelected;
	static void Draw(Rect rect, Graphics* graph);
	static void SetColor(COLORREF temp);
	static void SetWidth(float newWidth);
	static void Update();
	static void SetBackGroundColor(COLORREF temp);
	static Rect ReBalance(int x1, int y1, int x2, int y2);
	static void Default();
	static void Save(CString FilePath);
	static void Load(CString FilePath);
	static void UpdateData();
	static void Delete(Figure* Fig);
};

