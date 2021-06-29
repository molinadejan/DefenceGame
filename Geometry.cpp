#include "Geometry.h"

#define _USE_MATH_DEFINES
#include <math.h>

Point operator+(const Point& p1, const Point& p2)
{
	return { p1.x + p2.x, p1.y + p2.y };
}

Point operator-(const Point& p1, const Point& p2)
{
	return { p1.x - p2.x, p1.y - p2.y };
}

Point operator*(const Point& p, const double d)
{
	return { p.x * d, p.y *d };
}

Point operator*(const double d, const Point& p)
{
	return { p.x * d, p.y *d };
}

Point operator/(const Point& p, const double d)
{
	if (d == 0)
		return { p.x, p.y };

	return { p.x / d, p.y / d };
}

void operator+=(Point& p1, const Point& p2)
{
	p1.x += p2.x;
	p1.y += p2.y;
}

void operator-=(Point& p1, const Point& p2)
{
	p1.x -= p2.x;
	p1.y -= p2.y;
}

double Dot(const Point& p1, const Point& p2)
{
	return p1.x * p2.x + p1.y * p2.y;
}

void DrawGrid(HDC hdc, const Point &p, int colCnt, int rowCnt, int colGap, int rowGap)
{
	int bottom = p.y + rowCnt * rowGap;
	int right = p.x + colCnt * colGap;

	// col
	for (int i = p.x; i <= right; i += colGap)
	{
		MoveToEx(hdc, i, p.y, NULL);
		LineTo(hdc, i, bottom);
	}

	//row
	for (int i = p.y; i <= bottom; i += rowGap)
	{
		MoveToEx(hdc, p.x, i, NULL);
		LineTo(hdc, right, i);
	}
}

void DrawCircle(HDC hdc, const Point &p, int radius)
{
	Ellipse(hdc, p.x - radius, p.y - radius, p.x + radius, p.y + radius);
}

void DrawHalfCircle(HDC hdc, const Point & p, double radius)
{
	Pie(hdc, p.x - radius, p.y - radius, p.x + radius, p.y + radius, 
		p.x + radius, p.y, p.x - radius, p.y);
}

void DrawSunflowerByRadius(HDC hdc, const Point &center, int radius, int leafRadius)
{
	DrawCircle(hdc, center, radius);

	double angleRadian = 2 * asin((double)leafRadius / (radius + leafRadius));
	double angle = Rad2Deg(angleRadian);

	Point tmpP = { center.x, center.y - radius - leafRadius };

	for (double i = 0; i < 360; i += angle)
	{
		DrawCircle(hdc, tmpP, leafRadius);
		tmpP = Rotate(center, tmpP, angleRadian);
	}
}

void DrawSunflowerByCount(HDC hdc, const Point &center, int radius, int cnt)
{
	// 인접한 원의 개수는 최소 3개
	if (cnt < 3) cnt = 3;

	DrawCircle(hdc, center, radius);

	double angle = 360.0 / cnt;
	double angleRadian = Deg2Rad(angle);

	//double _leafRadius = radius * sqrt(2 - 2 * cos(angleRadian)) / (2 - sqrt(2 - 2 * cos(angleRadian)));
	double _leafRadius = radius * sin(angleRadian / 2) / (1 - sin(angleRadian / 2));

	int leafRadius = (int)floor(_leafRadius + 0.5);

	Point tmpP = { center.x, center.y - radius - leafRadius };

	for (double i = 0; i < 360; i += angle)
	{
		DrawCircle(hdc, tmpP, leafRadius);
		tmpP = Rotate(center, tmpP, angleRadian);
	}
}

void DrawRectangle(HDC hdc, const Point &p, int width, int height)
{
	Rectangle(hdc, p.x - width / 2, p.y - height / 2, p.x + width / 2, p.y + height / 2);
}

void DrawStar(HDC hdc, const Point &center, const int radius, int degree)
{
	double angleRadian = Deg2Rad(72);

	Point tip[10] = { {center.x, center.y - radius} };

	for (int i = 2; i < 10; i += 2)
		tip[i] = Rotate(center, tip[i - 2], angleRadian);

	bool check = false;

	for (int i = 0; i < 5; ++i)
		tip[2 * i + 1] = GetIntersection(tip[2 * i], tip[(2 * i + 4) % 10], tip[(2 * i + 8) % 10], tip[(2 * i + 2) % 10], check);

	if (degree != 0)
	{
		double angleRadian = Deg2Rad(degree);

		for (Point &p : tip)
			p = Rotate(center, p, angleRadian);
	}

	/*
	tip[1] = GetIntersection(tip[0], tip[4], tip[8], tip[2], check);
	tip[3] = GetIntersection(tip[2], tip[6], tip[0], tip[4], check);
	tip[5] = GetIntersection(tip[4], tip[8], tip[2], tip[6], check);
	tip[7] = GetIntersection(tip[6], tip[0], tip[4], tip[8], check);
	tip[9] = GetIntersection(tip[8], tip[2], tip[6], tip[0], check);
	*/

	//Polygon(hdc, tip, 10);
}

Point GetIntersection(const Point &p1, const Point &p2, const Point &p3, const Point &p4, bool &check)
{
	double slopeDiff = (p1.x - p2.x) * (p3.y - p4.y) - (p1.y - p2.y) * (p3.x - p4.x);

	if (slopeDiff == 0)
	{
		check = false;
		return { 0, 0 };
	}

	double tmpX = (p1.x * p2.y - p1.y * p2.x) * (p3.x - p4.x) - (p1.x - p2.x) * (p3.x * p4.y - p3.y * p4.x);
	double tmpY = (p1.x * p2.y - p1.y * p2.x) * (p3.y - p4.y) - (p1.y - p2.y) * (p3.x * p4.y - p3.y * p4.x);

	tmpX /= slopeDiff;
	tmpY /= slopeDiff;

	return { tmpX, tmpY };
}

Point Rotate(const Point &center, const Point &p, const double radian)
{
	Point tmp = { p.x - center.x, p.y - center.y };

	double tmpX = cos(radian) * tmp.x - sin(radian) * tmp.y;
	double tmpY = sin(radian) * tmp.x + cos(radian) * tmp.y;

	return { tmpX + center.x, tmpY + center.y };
}

double Rad2Deg(const double radian)
{
	return radian * 180.0f / M_PI;
}

double Deg2Rad(const double degree)
{
	return degree * M_PI / 180.0f;
}

double GetDistance(const Point & p1, const Point & p2)
{
	return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

double GetDistance(double p1x, double p1y, double p2x, double p2y)
{
	return sqrt(pow(p1x - p2x, 2) + pow(p1y - p2y, 2));
}
