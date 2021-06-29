#include "DefenceWall.h"

void DefenceWall::Draw(HDC hdc) const
{
	DrawRectangle(hdc, pos, width, height);
}
