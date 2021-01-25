#ifndef INCLUDED_GAME_DYNAMIC_OBJECT_H
#define INCLUDED_GAME_DYNAMIC_OBJECT_H

class Image;

class DynamicObject
{
public:
	enum Type
	{
		TYPE_1P,
		TYPE_2P,
		TYPE_ENEMY,

		TYPE_NONE, // 올바르지 않은 값
	};
	DynamicObject();
	void set(int x, int y, Type);
	void draw(const Image*);
	void update();
private:
	Type mType;
	int mX;
	int mY;
	// 이동 처리용
	int mDirectionX;
	int mDirectionY;
};





#endif