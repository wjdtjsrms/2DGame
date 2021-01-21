#ifndef INCLUDED_STATE_H
#define INCLUDED_STATE_H
#include "Array2D.h"

class Image;

class State {
public:
	State(const char* stageData, int size);
	~State();
	void update(int,int);
	void draw() const;
	bool hasCleared() const;
	void reset();
	static const int MAX_MOVE_COUNT = 15; // 0.5 √ 

private:
	class Object;
	void setSize();

	int mWidth,mHeight;
	Array2D< Object > mObjects;
	Image* mImage;
	int mMoveCount;
	char* mStageData;
	int mStageDataSize;
	
};
#endif