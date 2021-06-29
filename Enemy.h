#pragma once
#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Geometry.h"

class Enemy
{
private:
	Point pos;
	double speed;
	double radius;

	//Point points[7];

public:

	Enemy(const Point& _pos, double _speed, double _radius);


	Point GetPos() const { return pos; }
	double GetRadius() const { return radius; }

	void Update();
	void Draw(HDC hdc) const;
};

#endif // !__ENEMY_H__

