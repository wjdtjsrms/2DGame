#ifndef INCLUDED_STATE_H
#define INCLUDED_STATE_H
#include "Array2D.h"

class Image;
class StaticObject;
class DynamicObject;

class State {
public:
	// 0이면 2인용 1이면 1인용
	static const int STAGE_ID_2PLAYERS = 0;
	State(int stageID);
	~State();

	void update();
	void draw() const;
	// 시퀸스 쪽에서 요구되는 기능
	bool hasCleared() const;
	bool isAlive1P() const; //1P는 살아있는가?
	bool isAlive2P() const; //2P는 살아있는가?

private:
	// 움직이지 않는 객체 담당
	Array2D< StaticObject > mStaticObjects; 

	// 움직이는 객체 담당
	DynamicObject* mDynamicObjects;
	int mDynamicObjectNumber;

	int mStageID;
	Image* mImage;
};
#endif