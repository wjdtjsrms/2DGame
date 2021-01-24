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
		srcX = 0;
		srcY = 32;
	}
	else  // �ٴ�
	{
		srcX = 16;
		srcY = 32;
		floor = true;
	}
	image->draw(x * 16, y * 16, srcX, srcY, 16, 16); // ����� ���� �׸���.

	if (floor || 1) // �ٴ��� ���δٸ�
	{
		srcX = -1; // �� �ǵ� ��
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
			image->draw(x * 16, y * 16, srcX, srcY, 16, 16); // �� ���� ��ź�� �������� �׸���.
		}
	}

}
