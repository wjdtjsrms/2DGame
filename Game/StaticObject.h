#ifndef INCLUDED_GAME_STATIC_OBJECT_H
#define INCLUDED_GAME_STATIC_OBJECT_H

class Image;
class DynamicObject;
class StaticObject
{
public:
	enum Flag
	{
		FLAG_WALL = (1 << 0), // ��ũ��Ʈ (�� ����)
		FLAG_BRICK = (1 << 1), // ����
		FLAG_BOMB = (1 << 2),  // ��ġ�Ǵ� ��ź
		FLAG_ITEM_BOMB = (1 << 3), // ��ź ������
		FLAG_ITEM_POWER = (1 << 4), // ��ǳ ������
		FLAG_FIRE_X = (1 << 5), // X ����
		FLAG_FIRE_Y = (1 << 6), // Y ����
		FLAG_EXPLODING = (1 << 7), // ���� ���� ��ġ
	};
	StaticObject();

	// �ٴ�, ��, ������ �׸��� ��ź�̳� �������� ������ �װ͵� �׸���.
	void draw(int x, int y, const Image*) const;
	void drawExplosion(int x, int y, const Image*) const;
	bool checkFlag(unsigned) const;
	void setFlag(unsigned);
	void resetFlag(unsigned);

	// ���⼭�� �Ⱦ��� state ���� ���µ� �������� �������� �𸣰ڴ�.
	int mCount;
	DynamicObject* mBombOwner;

private:
	unsigned mFlags;

};
#endif