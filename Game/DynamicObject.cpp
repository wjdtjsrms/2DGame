#include "GameLib/GameLib.h"
#include "GameLib/Framework.h"
using namespace GameLib;

#include "Game/DynamicObject.h"
#include "Image.h"

namespace // 번역 단위 유효범위 
{
	const int PLAYER_SPEED = 1000;
	const int ENEMY_SPEED = 500;
}

DynamicObject::DynamicObject()
	:mType(TYPE_NONE),mX(0xffffffff),mY(0xffffffff)
	,mDirectionX(0),mDirectionY(0)
{
}

void DynamicObject::set(int x, int y, Type type)
{
	// 내부 좌표 값으로 변경
	mX = x * 16000 + 8000;
	mY = y * 16000 + 8000;
	mType = type;

	if (type == TYPE_ENEMY)
	{
		mDirectionX = mDirectionY = 0;
		Framework f = Framework::instance();
		switch (f.getRandom(4)) {
		case 0: mDirectionX = 1; break;
		case 1: mDirectionX = -1; break;
		case 2: mDirectionY = 1; break;
		case 3: mDirectionY = -1; break;
		}
	}
}

void DynamicObject::draw(const Image* image)
{
	// 내부 좌표를 픽셀 좌표로 변환 (+500은 반올림)
	int dstX = (mX - 8000 + 500) / 1000;
	int dstY = (mY - 8000 + 500) / 1000;

	int srcX = -1, srcY = -1;
	switch (mType) {
	case TYPE_1P: srcX = 0; srcY = 0; break;
	case TYPE_2P: srcX = 16; srcY = 0; break;
	case TYPE_ENEMY: srcX = 32; srcY = 16; break;
	default: HALT("BABO TYPE"); break;
	}
	image->draw(dstX, dstY, srcX, srcY, 16, 16);
}

void DynamicObject::update() 
{
	Framework f = Framework::instance();

	if (mType == TYPE_ENEMY)
	{
		mX += mDirectionX * ENEMY_SPEED;
		mY += mDirectionY * ENEMY_SPEED;
	}
	else if (mType == TYPE_1P)
	{
		int dx, dy;
		dx = dy = 0;
		if (f.isKeyOn('w')) {
			dy = -1;
		}
		else if (f.isKeyOn('s')) {
			dy = 1;
		}
		else if (f.isKeyOn('a')) {
			dx = -1;
		}
		else if (f.isKeyOn('d')) {
			dx = 1;
		}
		mX += dx * PLAYER_SPEED;
		mY += dy * PLAYER_SPEED;
	}
	else if (mType == TYPE_2P) 
	{
		int dx, dy;
		dx = dy = 0;
		if (f.isKeyOn('i')) {
			dy = -1;
		}
		else if (f.isKeyOn('k')) {
			dy = 1;
		}
		else if (f.isKeyOn('j')) {
			dx = -1;
		}
		else if (f.isKeyOn('l')) {
			dx = 1;
		}
		mX += dx * PLAYER_SPEED;
		mY += dy * PLAYER_SPEED;
	}

	const int X_MIN = 8000; // 원점이 가운데있다.
	const int X_MAX = 320 * 1000 - 8000; // WIDTH 320px
	const int Y_MIN = 8000;
	const int Y_MAX = 240 * 1000 - 8000; // HEIGHT 240px
	bool hit = false;

	// 화면 밖으로 나가지 못하게 한다.
	if (mX < X_MIN) {
		mX = X_MIN;
		hit = true;
	}
	else if (mX > X_MAX) {
		mX = X_MAX;
		hit = true;
	}
	if (mY < Y_MIN) {
		mY = Y_MIN;
		hit = true;
	}
	else if (mY > Y_MAX) {
		mY = Y_MAX;
		hit = true;
	}
	// enemy는 벽에 hit 될때까지 전진한다.
	if (hit && mType == TYPE_ENEMY) {
		mDirectionX = mDirectionY = 0;
		switch (f.getRandom(4)) {
		case 0: mDirectionX = 1; break;
		case 1: mDirectionX = -1; break;
		case 2: mDirectionY = 1; break;
		case 3: mDirectionY = -1; break;
		}
	}
}