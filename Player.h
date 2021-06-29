#pragma once

#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Geometry.h"

class Player
{
private:

	// ��ġ
	Point pos;

	// �ѱ��� �̷�� ��, �߽��� �������� �������� +, ������ -
	int angle;
	double radius;

	// �ѱ��� ����, ��
	Point muzzleStart;
	Point muzzleEnd;

	void DrawBody(HDC hdc) const;
	void DrawMuzzle(HDC hdc) const;

public:

	Player(const Point& _pos, double _radius) : pos(_pos), angle(90), radius(_radius), muzzleStart({ _pos.x, _pos.y - radius }), muzzleEnd({ _pos.x, _pos.y - radius * 2 }) { }

	Point GetShotPoint() const { return muzzleEnd; }
	Point GetDir() const { return (muzzleEnd - muzzleStart) / radius; }

	void DrawPlayer(HDC hdc) const;
	void MuzzleRotate(int degree);
};

#endif // !__PLAYER_H__
