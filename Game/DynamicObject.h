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

		TYPE_NONE, // �ùٸ��� ���� ��
	};
	DynamicObject();
	void set(int x, int y, Type);
	void draw(const Image*);

	// Dynamic object�� static object ������ ����
	void move(const int* wallsX, int* wallsY, int wallNumber);
	void getCell(int* x, int* y) const;
	// ���̳��� ������Ʈ ������ �浹 ����
	void doCollisionReactionToDynamic(DynamicObject*);
	// ���� �ȵ�
	int getIntersectionCell(int* xArray, int* yArray, int dx, int dy) const;
	void doCollisionReactionToStatic(StaticObject** o, int cellNumber);


	// AABB �浹 ����
	bool isIntersect(const DynamicObject&) const;
	// ���α��� ĸ��ȭ
	bool isIntersectWall(int wallCellX, int wallCellY);

	bool hasBombButtonPressed() const;	// ��ź ��ġ ��� �Լ�
	bool isPlayer() const;
	bool isEnemy() const;
	void die(); // �÷��̾� ���¸� die�� �ٲ۴�.
	bool isDead() const; 

	Type mType;
	int mBombPower; 
	int mBombNumber; 
	int mPlayerID; // 1�ο�� 2�ο� ����
	int mLastBombX[2]; // ������ ��ź ��ġ�� ����Ѵ�.
	int mLastBombY[2]; // ���� 1�� �ٲٸ� ����ִ� ���� �����.
private:
	// ���̶� �浹 ����
	static bool isIntersectWall(int x, int y, int wallCellX, int wallCellY);
	// �÷��̾�� ���� ���� ���� �ӵ� ���� 
	void getVelocity(int* dx, int* dy) const;
	// �÷��̾�� ���� �̵� ���� ���� 
	void getDirection(int* dx, int* dy) const;
	int mX;
	int mY;
	// �̵� ó����
	int mDirectionX;
	int mDirectionY;
};
#endif