#pragma once

#ifndef __BULLET_H__
#define __BULLET_H__

#include "Geometry.h"
#include <vector>

class Bullet
{
private:

	Point pos;
	Point dir;
	
	double speed;
	double radius;

public:
	Bullet(const Point& _pos, const Point& _dir, double _speed, double _radius)
		: pos(_pos), dir(_dir), speed(_speed), radius(_radius) { }

	void Update();

	void Draw(HDC hdc) const;

	Point GetPos() const { return pos; }
	double GetRadius() const { return radius; }
};

#endif // !__BULLET_H__
