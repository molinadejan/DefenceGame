#include "Player.h"

void Player::DrawBody(HDC hdc) const
{
	DrawHalfCircle(hdc, pos, radius);
}

void Player::DrawMuzzle(HDC hdc) const
{
	MoveToEx(hdc, muzzleStart.x, muzzleStart.y, NULL);
	LineTo(hdc, muzzleEnd.x, muzzleEnd.y);
}

void Player::DrawPlayer(HDC hdc) const
{
	DrawBody(hdc);
	DrawMuzzle(hdc);
}

void Player::MuzzleRotate(int degree)
{
	if (angle + degree < 0 || angle + degree > 180)
		return;

	angle += degree;

	muzzleStart = Rotate(pos, muzzleStart, Deg2Rad(degree));
	muzzleEnd = Rotate(pos, muzzleEnd, Deg2Rad(degree));
}
