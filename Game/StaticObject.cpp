#include "GameLib\GameLib.h"
#include "GameLib\Framework.h"
using namespace GameLib;

#include "Game\StaticObject.h"
#include "Image.h"

StaticObject::StaticObject()
	:mFlags(0),
	mCount(0),
	mBombOwner(0)
{}

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
		if (mFlags & (FLAG_FIRE_X | FLAG_FIRE_Y)) // 벽돌이 폭탄 범위에 있으면
		{
			srcX = 0;
			srcY = 48;
		}
		else
		{
			srcX = 0;
			srcY = 32;
		}
	}
	else  // 바닥
	{
		srcX = 16;
		srcY = 32;
		floor = true;
	}
	image->draw(x * 16, y * 16, srcX, srcY, 16, 16); // 배경을 먼저 그린다.

	if (floor) // 바닥이 보인다면
	{
		srcX = -1; // 값 판독 용
		if ((mFlags & FLAG_BOMB) && !(mFlags & FLAG_EXPLODING)) { // 폭탄을 그린다.
			srcX = 32;
			srcY = 32;
		}
		else if (mFlags & FLAG_ITEM_BOMB) {
			srcX = 32;
			srcY = 0;
		}
		else if (mFlags & FLAG_ITEM_POWER) {
			srcX = 48;
			srcY = 0;
		}
		if (srcX != -1) // 뭐라도 플래그에 잡혀야 그림
		{
			image->draw(x * 16, y * 16, srcX, srcY, 16, 16); // 바닥 그리고 폭탄과 아이템을 그린다.
		}
	}

}

void StaticObject::drawExplosion(int x, int y, const Image* image) const
{
	int srcX = -1;
	int srcY = -1;

	if (!(mFlags & FLAG_WALL) && !(mFlags & FLAG_BRICK)) // 폭풍 범위에 들어간 벽과 콘크리트에는 폭풍을 그리지 않는다. 근데 그려도 예쁨
	{
		if (mFlags & FLAG_EXPLODING) // + 모양 폭풍
		{
			srcX = 48;
			srcY = 32;
		}
		else if (mFlags & FLAG_FIRE_X) {
			if (mFlags & FLAG_FIRE_Y) {
				srcX = 48;
				srcY = 32;
			}
			else {
				srcX = 0; // - 모양 폭풍
				srcY = 16;
			}
		}
		else if (mFlags & FLAG_FIRE_Y) { // I 모양 폭풍
			srcX = 16;
			srcY = 16;
		}

	}

	if (srcX != -1) // 뭐라도 플래그에 잡혀야 그림
	{
		image->draw(x * 16, y * 16, srcX, srcY, 16, 16);
	}
}
