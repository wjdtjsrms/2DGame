#ifndef INCLUDED_STATE_H
#define INCLUDED_STATE_H
#include "Array2D.h"

class Image;
class StaticObject;

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
	Array2D< StaticObject > mStaticObjects; // �������� �ʴ� ��ü ���
	int mStageID;

	Image* mImage;
	
};
#endif