#ifndef INCLUDED_GAME_STATIC_OBJECT_H
#define INCLUDED_GAME_STATIC_OBJECT_H

class Image;
class DynamicObject;
class StaticObject
{
public:
	enum Flag
	{
		FLAG_WALL = (1 << 0), // 콘크리트 (안 깨짐)
		FLAG_BRICK = (1 << 1), // 벽돌
		FLAG_BOMB = (1 << 2),  // 설치되는 폭탄
		FLAG_ITEM_BOMB = (1 << 3), // 폭탄 아이템
		FLAG_ITEM_POWER = (1 << 4), // 폭풍 아이템
		FLAG_FIRE_X = (1 << 5), // X 폭발
		FLAG_FIRE_Y = (1 << 6), // Y 폭발
		FLAG_EXPLODING = (1 << 7), // 폭발 시작 위치
	};
	StaticObject();

	// 바닥, 벽, 벽돌을 그리고 폭탄이나 아이템이 있으면 그것도 그린다.
	void draw(int x, int y, const Image*) const;
	void drawExplosion(int x, int y, const Image*) const;
	bool checkFlag(unsigned) const;
	void setFlag(unsigned);
	void resetFlag(unsigned);

	// 여기서는 안쓰고 state 가서 쓰는데 뭣때문에 쓰는지는 모르겠다.
	int mCount;
	DynamicObject* mBombOwner;

private:
	unsigned mFlags;

};
#endif