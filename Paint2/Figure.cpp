#include "stdafx.h"
#include "Figure.h"

Figure::Figure(Color PenColor, Color BackgroundColor, float PenWidth)
{
	m_Color = PenColor;
	m_Background_Color = BackgroundColor;
	m_Width = PenWidth;
}

void Figure::SetWidth(float newWidth)
{
	m_Width = newWidth;
}

void Figure::SetColor(Color newColor)
{
	m_Color = newColor;
}

void Figure::SetBackgroundColor(Color color)
{
	m_Background_Color = color;
}

float Figure::GetWidth()
{
	return m_Width;
}

Color Figure::GetColor()
{
	return m_Color;
}

Color Figure::GetBackgroundColor()
{
	return m_Background_Color;
}