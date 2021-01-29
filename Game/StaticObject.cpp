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
	// | ��Ծ����� �ΰ��� �ȱ׷�����
	mFlags |= f;
}

void StaticObject::resetFlag(unsigned f)
{
	mFlags &= ~f; // ~f f�� not ����
}


void StaticObject::draw(int x, int y, const Image* image) const
{
	int srcX = -1;
	int srcY = -1;
	bool floor = false;

	if (mFlags & FLAG_WALL) // ��ũ��Ʈ
	{
		srcX = 48;
		srcY = 16;
	}
	else if (mFlags & FLAG_BRICK) // ����
	{
		if (mFlags & (FLAG_FIRE_X | FLAG_FIRE_Y)) // ������ ��ź ������ ������
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
	else  // �ٴ�
	{
		srcX = 16;
		srcY = 32;
		floor = true;
	}
	image->draw(x * 16, y * 16, srcX, srcY, 16, 16); // ����� ���� �׸���.

	if (floor) // �ٴ��� ���δٸ�
	{
		srcX = -1; // �� �ǵ� ��
		if ((mFlags & FLAG_BOMB) && !(mFlags & FLAG_EXPLODING)) { // ��ź�� �׸���.
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
		if (srcX != -1) // ���� �÷��׿� ������ �׸�
		{
			image->draw(x * 16, y * 16, srcX, srcY, 16, 16); // �ٴ� �׸��� ��ź�� �������� �׸���.
		}
	}

}

void StaticObject::drawExplosion(int x, int y, const Image* image) const
{
	int srcX = -1;
	int srcY = -1;

	if (!(mFlags & FLAG_WALL) && !(mFlags & FLAG_BRICK)) // ��ǳ ������ �� ���� ��ũ��Ʈ���� ��ǳ�� �׸��� �ʴ´�. �ٵ� �׷��� ����
	{
		if (mFlags & FLAG_EXPLODING) // + ��� ��ǳ
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
				srcX = 0; // - ��� ��ǳ
				srcY = 16;
			}
		}
		else if (mFlags & FLAG_FIRE_Y) { // I ��� ��ǳ
			srcX = 16;
			srcY = 16;
		}

	}

	if (srcX != -1) // ���� �÷��׿� ������ �׸�
	{
		image->draw(x * 16, y * 16, srcX, srcY, 16, 16);
	}
}
