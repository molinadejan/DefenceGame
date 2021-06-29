#pragma once

#ifndef __DEFENCEWALL_H_
#define __DEFENCEWALL_H_

#include "Geometry.h"

class DefenceWall
{
private:
	Point pos;

	double width; 
	double height;

public:

	DefenceWall(const Point& _pos, const double _width, const double _height) : pos(_pos), width(_width), height(_height) { }
	
	void Draw(HDC hdc) const;
	Point GetPos() const { return pos; }
	double GetWidth() const { return width; }
	double GetHeight() const { return height; }
};

#endif // !__DEFENCEWALL_H_

