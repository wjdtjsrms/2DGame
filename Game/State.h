#ifndef INCLUDED_STATE_H
#define INCLUDED_STATE_H
#include "Array2D.h"

class Image;
class StaticObject;
class DynamicObject;

class State {
public:
	// 0�̸� 2�ο� 1�̸� 1�ο�
	static const int STAGE_ID_2PLAYERS = 0;
	State(int stageID);
	~State();

	void update();
	void draw() const;
	// ������ �ʿ��� �䱸�Ǵ� ���
	bool hasCleared() const;
	bool isAlive1P() const; //1P�� ����ִ°�?
	bool isAlive2P() const; //2P�� ����ִ°�?

private:
	// �������� �ʴ� ��ü ���
	Array2D< StaticObject > mStaticObjects; 

	// �����̴� ��ü ���
	DynamicObject* mDynamicObjects;
	int mDynamicObjectNumber;

	int mStageID;
	Image* mImage;
};
#endif