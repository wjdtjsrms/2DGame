#include "GameLib/GameLib.h"
#include "GameLib/Framework.h"
using namespace GameLib;

#include "Game/DynamicObject.h"
#include "Image.h"

namespace // ���� ���� ��ȿ���� 
{
	const int PLAYER_SPEED = 1000;
	const int ENEMY_SPEED = 500;
	static const int HALF_SIZE = 6000; // ���� �γ��ϰ� ������ ��Ҵ�.

	 // �����浹�� ���м��� ���� x�� 1000�� Ȯ���ߴ�. ������ �߽��̴�.			  
	int convertCellToInner(int x) {
		return x * 16000 + 8000;  // �߽� ���� ��ȯ
	}
	int convertInnerToPixel(int x) {
		// ���� ��ǥ�� �ȼ� ��ǥ�� ��ȯ (+500�� �ݿø�)
		return  (x - 8000 + 500) / 1000;
	}
}

DynamicObject::DynamicObject()
	:mType(TYPE_NONE),
	mBombPower(2),
	mBombNumber(2),
	mPlayerID(0xffffffff),
	mX(-1),
	mY(-1),
	mDirectionX(0),
	mDirectionY(0) {
	mLastBombX[0] = mLastBombX[1] = -1;
	mLastBombY[0] = mLastBombY[1] = -1;
}

void DynamicObject::set(int x, int y, Type type)
{
	// ���� ��ǥ ������ ����
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
	if (isDead())
	{
		return;
	}

	// ���� ��ǥ�� �ȼ� ��ǥ�� ��ȯ (+500�� �ݿø�)
	int dstX = convertInnerToPixel(mX);
	int dstY = convertInnerToPixel(mY);

	int srcX = -1, srcY = -1;
	switch (mType)
	{
	case TYPE_PLAYER:
		switch (mPlayerID)
		{
		case 0: srcX = 0; srcY = 0; break;
		case 1: srcX = 16; srcY = 0; break;
		}
		break;
	case TYPE_ENEMY: srcX = 32; srcY = 16; break;
		break;
	default: HALT("BABO TYPE"); break;
	}
	image->draw(dstX, dstY, srcX, srcY, 16, 16);
}


void DynamicObject::move(const int* wallsX, int* wallsY, int wallNumber)
{
	int dx, dy;
	getVelocity(&dx, &dy);

	int movedX = mX + dx;
	int movedY = mY + dy;
	bool hitX = false;
	bool hitY = false;
	bool hit = false;

	for (int i = 0; i < wallNumber; ++i)
	{
		if (isIntersectWall(movedX, mY, wallsX[i], wallsY[i]))
		{
			hitX = hit = true;
		}
		if (isIntersectWall(mX, movedY, wallsX[i], wallsY[i]))
		{
			hitY = hit = true;
		}
	}

	// �浹�� �������� �ʴ´ٸ� ä��
	if (hitX && !hitY)
	{
		mY = movedY;
	}
	else if (!hitX && hitY)
	{
		mX = movedX;
	}
	else // ���������� �浹�� �Ͼ����
	{
		for (int i = 0; i < wallNumber; ++i)
		{
			if (isIntersectWall(movedX, movedY, wallsX[i], wallsY[i])) {
				hit = true;
			}
			if (!hit) {
				mX = movedX;
				mY = movedY;
			}
		}
	}

	// ���� �ε�ġ�� ������ �ٲ۴�.
	if (hit && mType == TYPE_ENEMY) {
		mDirectionX = mDirectionY = 0;
		switch (Framework::instance().getRandom(4)) {
		case 0: mDirectionX = 1; break;
		case 1: mDirectionX = -1; break;
		case 2: mDirectionY = 1; break;
		case 3: mDirectionY = -1; break;
		}
	}
}


// �����̴� ��ü���� �浹 ó��
void DynamicObject::doCollisionReactionToDynamic(DynamicObject* another)
{
	if (another->isDead())
	{
		return;
	}
	DynamicObject& o1 = *this;
	DynamicObject& o2 = *another;

	if (o1.isIntersect(o2)) // �浹 ����
	{
		// ���̶� �浹�ϸ� ����
		if (o1.isPlayer() && o2.isEnemy())
		{
			o1.die();
		}
		else if (o1.isEnemy() && o2.isPlayer())
		{
			o2.die();
		}
	}

}

// AABB �浹 ����
bool DynamicObject::isIntersect(const DynamicObject& o) const {
	int al = mX - HALF_SIZE; //left A
	int ar = mX + HALF_SIZE; //right A
	int bl = o.mX - HALF_SIZE; //left B
	int br = o.mX + HALF_SIZE; //right B
	if ((al < br) && (ar > bl)) {
		int at = mY - HALF_SIZE; //top A
		int ab = mY + HALF_SIZE; //bottom A
		int bt = o.mY - HALF_SIZE; //top B
		int bb = o.mY + HALF_SIZE; //bottom B
		if ((at < bb) && (ab > bt)) {
			return true;
		}
	}
	return false;

}

bool DynamicObject::isIntersectWall(int wallX, int wallY) {
	return isIntersectWall(mX, mY, wallX, wallY);
}

bool DynamicObject::isIntersectWall(int x, int y, int wallX, int wallY) {
	int wx = convertCellToInner(wallX);
	int wy = convertCellToInner(wallY);

	// ������ �߽����� �� AABB ����
	int al = x - HALF_SIZE; //left A
	int ar = x + HALF_SIZE; //right A
	int bl = wx - 8000; //left B
	int br = wx + 8000; //right B
	if ((al < br) && (ar > bl)) {
		int at = y - HALF_SIZE; //top A
		int ab = y + HALF_SIZE; //bottom A
		int bt = wy - 8000; //top B
		int bb = wy + 8000; //bottom B
		if ((at < bb) && (ab > bt)) {
			return true;
		}
	}
	return false;
}

bool DynamicObject::hasBombButtonPressed() const {
	Framework f = Framework::instance();
	if (mType == TYPE_PLAYER) {
		if (mPlayerID == 0) {
			return f.isKeyOn('v');
		}
		else if (mPlayerID == 1) {
			return f.isKeyOn('l');
		}
	}
	return false;
}

bool DynamicObject::isPlayer() const {
	return (mType == TYPE_PLAYER);
}

bool DynamicObject::isEnemy() const {
	return (mType == TYPE_ENEMY);
}

void DynamicObject::die() {
	mType = TYPE_NONE;
}

bool DynamicObject::isDead() const {
	return (mType == TYPE_NONE);
}

// �迭 �� ��ȯ
void DynamicObject::getCell(int* x, int* y) const {
	*x = mX / 16000;
	*y = mY / 16000;
}

void DynamicObject::getVelocity(int* dx, int* dy) const {
	int speedX, speedY;
	if (mType == TYPE_ENEMY) {
		speedX = ENEMY_SPEED;
		speedY = ENEMY_SPEED;
	}
	else {
		speedX = PLAYER_SPEED;
		speedY = PLAYER_SPEED;
	}

	getDirection(dx, dy);

	*dx = *dx * speedX;
	*dy = *dy * speedY;

}

void DynamicObject::getDirection(int * dx, int * dy) const
{
	Framework f = Framework::instance();
	*dx = *dy = 0;

	if (mType == TYPE_PLAYER)
	{
		if (mPlayerID == 0)
		{
			if (f.isKeyOn('w')) {
				*dy = -1;
			}
			else if (f.isKeyOn('s')) {
				*dy = 1;
			}
			else if (f.isKeyOn('a')) {
				*dx = -1;
			}
			else if (f.isKeyOn('d')) {
				*dx = 1;
			}
		}
		else if (mPlayerID == 1)
		{
			if (f.isKeyOn('i')) {
				*dy = -1;
			}
			else if (f.isKeyOn('k')) {
				*dy = 1;
			}
			else if (f.isKeyOn('j')) {
				*dx = -1;
			}
			else if (f.isKeyOn('l')) {
				*dx = 1;
			}

		}
	}
	else if (mType == TYPE_ENEMY)
	{
		*dx += mDirectionX;
		*dy += mDirectionY;
	}
}


