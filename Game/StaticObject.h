#ifndef INCLUDED_GAME_STATIC_OBJECT_H
#define INCLUDED_GAME_STATIC_OBJECT_H

class Image;

class StaticObject
{
public:
	enum Flag
	{
		FLAG_WALL = (1 << 0), // ��ũ��Ʈ (�� ����)
		FLAG_BRICK = (1 << 1), // ����
		FLAG_ITEM_BOMB = (1 << 2), // ��ź ������
		FLAG_ITEM_POWER = (1 << 3), // ��ǳ ������
	};
	StaticObject();

	// �ٴ�, ��, ������ �׸��� ��ź�̳� �������� ������ �װ͵� �׸���.
	void draw(int x, int y, const Image*) const;
	bool checkFlag(unsigned) const;
	void setFlag(unsigned);
	void resetFlag(unsigned);

private:
	unsigned mFlags;

};
#endif