#include "Enemy.h"

Enemy::Enemy(const Point & _pos, double _speed, double _radius)
	: pos(_pos), speed(_speed), radius(_radius)
{
	/*points[0] = {pos.x - 15, pos.y - 45};
	points[1] = {pos.x + 15, pos.y - 45};
	points[2] = {pos.x + 15, pos.y + 15};
	points[3] = {pos.x + 30, pos.y + 15};
	points[4] = {pos.x     , pos.y + 45};
	points[5] = {pos.x - 30, pos.y + 15};
	points[6] = {pos.x - 15, pos.y + 15};*/
}

void Enemy::Update()
{
	pos += Point{ 0, 1 } * speed;
}

void Enemy::Draw(HDC hdc) const
{
	DrawCircle(hdc, pos, radius);

	/*for (int i = 0; i < 7; ++i)
	{
		MoveToEx(hdc, points[i].x + pos.x, points[i].y + pos.y, NULL);
		LineTo(hdc, points[(i + 1) % 7].x + pos.x, points[(i + 1) % 7].y + pos.y);
	}*/
}
