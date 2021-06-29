#include "Bullet.h"

void Bullet::Update()
{
	pos += dir * speed;
}

void Bullet::Draw(HDC hdc) const
{
	DrawCircle(hdc, pos, radius);
}
