#ifndef INCLUDED_GAME_DYNAMIC_OBJECT_H
#define INCLUDED_GAME_DYNAMIC_OBJECT_H

class Image;
class StaticObject;

class DynamicObject
{
public:
	enum Type
	{

		TYPE_PLAYER,
		TYPE_ENEMY,

		TYPE_NONE, // 올바르지 않은 값
	};
	DynamicObject();
	void set(int x, int y, Type);
	void draw(const Image*);

	// Dynamic object와 static object 끼리의 판정
	void move(const int* wallsX, int* wallsY, int wallNumber);
	void getCell(int* x, int* y) const;
	// 다이나믹 오브젝트 끼리의 충돌 검출
	void doCollisionReactionToDynamic(DynamicObject*);
	// 구현 안됨
	int getIntersectionCell(int* xArray, int* yArray, int dx, int dy) const;
	void doCollisionReactionToStatic(StaticObject** o, int cellNumber);


	// AABB 충돌 검출
	bool isIntersect(const DynamicObject&) const;
	// 세부구현 캡슐화
	bool isIntersectWall(int wallCellX, int wallCellY);

	bool hasBombButtonPressed() const;	// 폭탄 설치 담당 함수
	bool isPlayer() const;
	bool isEnemy() const;
	void die(); // 플레이어 상태를 die로 바꾼다.
	bool isDead() const; 

	Type mType;
	int mBombPower; 
	int mBombNumber; 
	int mPlayerID; // 1인용과 2인용 구별
	int mLastBombX[2]; // 마지막 폭탄 위치를 기억한다.
	int mLastBombY[2]; // 값을 1로 바꾸면 재미있는 일이 생긴다.
private:
	// 벽이랑 충돌 검출
	static bool isIntersectWall(int x, int y, int wallCellX, int wallCellY);
	// 플레이어와 적의 방향 포함 속도 리턴 
	void getVelocity(int* dx, int* dy) const;
	// 플레이어와 적의 이동 방향 리턴 
	void getDirection(int* dx, int* dy) const;
	int mX;
	int mY;
	// 이동 처리용
	int mDirectionX;
	int mDirectionY;
};
#endif