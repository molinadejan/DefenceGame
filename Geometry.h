#pragma once

#include <windows.h>

#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

// ÁÂÇ¥ ±¸Á¶Ã¼
struct Point
{
	double x, y;
};

Point operator+(const Point& p1, const Point& p2);
Point operator-(const Point& p1, const Point& p2);

Point operator*(const Point& p, const double d);
Point operator*(const double d, const Point& p);

Point operator/(const Point& p, const double d);

void operator+=(Point& p1, const Point& p2);
void operator-=(Point& p1, const Point& p2);

double Dot(const Point& p1, const Point& p2);

void DrawGrid(HDC hdc, const Point &p, int colCnt, int rowCnt, int colGap, int rowGap);
void DrawCircle(HDC hdc, const Point &p, int radius);
void DrawHalfCircle(HDC hdc, const Point &p, double radius);
void DrawSunflowerByRadius(HDC hdc, const Point &center, int radius, int leafRadius);
void DrawSunflowerByCount(HDC hdc, const Point &center, int radius, int cnt);
void DrawRectangle(HDC hdc, const Point &p, int width, int height);
void DrawStar(HDC hdc, const Point &center, const int radius, int degree);

Point GetIntersection(const Point &p1, const Point &p2, const Point &p3, const Point &p4, bool &check);

Point Rotate(const Point &center, const Point &p, const double radian);

double Rad2Deg(const double radian);
double Deg2Rad(const double degree);

double GetDistance(const Point &p1, const Point &p2);
double GetDistance(double p1x, double p1y, double p2x, double p2y);

#endif // !__GEOMETRY_H__