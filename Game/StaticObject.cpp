#include "GameLib\GameLib.h"
using namespace GameLib;

#include "Game\StaticObject.h"
#include "Image.h"

StaticObject::StaticObject()
	:mFlags(0){}

bool StaticObject::checkFlag(unsigned f) const
{
	return (f & mFlags) ? true : false;
}

void StaticObject::setFlag(unsigned f)
{
	// | 까먹었더니 두개가 안그려지네
	mFlags |= f;
}

void StaticObject::resetFlag(unsigned f)
{
	mFlags &= ~f; // ~f f의 not 연산
}


void StaticObject::draw(int x, int y, const Image* image) const
{
	int srcX = -1;
	int srcY = -1;
	bool floor = false;

	if (mFlags & FLAG_WALL) // 콘크리트
	{ 
		srcX = 48;
		srcY = 16;
	}
	else if (mFlags & FLAG_BRICK) // 벽돌
	{
		srcX = 0;
		srcY = 32;
	}
	else  // 바닥
	{
		srcX = 16;
		srcY = 32;
		floor = true;
	}
	image->draw(x * 16, y * 16, srcX, srcY, 16, 16); // 배경을 먼저 그린다.

	if (floor || 1) // 바닥이 보인다면
	{
		srcX = -1; // 값 판독 용
		if (mFlags & FLAG_ITEM_BOMB) 
		{
			srcX = 32;
			srcY = 0;
		}
		else if (mFlags & FLAG_ITEM_POWER)
		{
			srcX = 48;
			srcY = 0;
		}
		if (srcX != -1)
		{
			image->draw(x * 16, y * 16, srcX, srcY, 16, 16); // 그 다음 폭탄과 아이템을 그린다.
		}
	}

}
