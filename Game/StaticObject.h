#ifndef INCLUDED_GAME_STATIC_OBJECT_H
#define INCLUDED_GAME_STATIC_OBJECT_H

class Image;

class StaticObject
{
public:
	enum Flag
	{
		FLAG_WALL = (1 << 0), // ÄÜÅ©¸®Æ® (¾È ±úÁü)
		FLAG_BRICK = (1 << 1), // º®µ¹
		FLAG_ITEM_BOMB = (1 << 2), // ÆøÅº ¾ÆÀÌÅÛ
		FLAG_ITEM_POWER = (1 << 3), // ÆøÇ³ ¾ÆÀÌÅÛ
	};
	StaticObject();

	// ¹Ù´Ú, º®, º®µ¹À» ±×¸®°í ÆøÅºÀÌ³ª ¾ÆÀÌÅ×ÀÌ ÀÖÀ¸¸é ±×°Íµµ ±×¸°´Ù.
	void draw(int x, int y, const Image*) const;
	bool checkFlag(unsigned) const;
	void setFlag(unsigned);
	void resetFlag(unsigned);

private:
	unsigned mFlags;

};
#endif