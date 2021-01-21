#include "GameLib\Framework.h"
using namespace GameLib;

#include "State.h"
#include "Image.h"


class State::Object {
public:
	enum Type {
		OBJ_SPACE,
		OBJ_WALL,
		OBJ_BLOCK,
		OBJ_MAN,

		OBJ_UNKNOWN,
	};
	enum ImageID {
		IMAGE_ID_PLAYER,
		IMAGE_ID_WALL,
		IMAGE_ID_BLOCK,
		IMAGE_ID_GOAL,
		IMAGE_ID_SPACE,
	};

	Object() :mType(OBJ_WALL), mGoalFlag(false), mMoveX(0), mMoveY(0) {}

	// 스테이지 테이터의 문자로 자신을 초기화
	void set(char c) {
		switch (c) {
		case '#': mType = OBJ_WALL; break;
		case ' ': mType = OBJ_SPACE; break;
		case 'o': mType = OBJ_BLOCK; break;
		case 'O': mType = OBJ_BLOCK; mGoalFlag = true; break;
		case '.': mType = OBJ_SPACE; mGoalFlag = true; break;
		case 'p': mType = OBJ_MAN; break;
		case 'P': mType = OBJ_MAN; mGoalFlag = true; break;
		}
	}
	// 배경 전경 순으로 그리기
	void drawBackground(int x, int y, const Image* image) const {
		// 바닥 아니면 벽 그리기
		if (mType == OBJ_WALL) {
			drawCell(x, y, IMAGE_ID_WALL, image);
		}
		else {
			if (mGoalFlag) {
				drawCell(x, y, IMAGE_ID_GOAL, image);
			}
			else {
				drawCell(x, y, IMAGE_ID_SPACE, image);
			}
		}
	}
	void drawForeGround(int x, int y, const Image* image, int moveCount) const
	{
		ImageID id = IMAGE_ID_SPACE; 

		// 전경 그리기
		if (mType == OBJ_BLOCK) {
			id = IMAGE_ID_BLOCK;
		}
		else if (mType == OBJ_MAN) {
			id = IMAGE_ID_PLAYER;
		}
		// 움직이는 물체 표현?
		if (id != IMAGE_ID_SPACE) { 
			const int m = State::MAX_MOVE_COUNT;
			int dx = ( mMoveX * (m - moveCount) * 32) / m;
			int dy = ( mMoveY * (m - moveCount) * 32) / m;
			image->draw(x * 32 - dx, y * 32 - dy, id * 32, 0, 32, 32);
		}
	}

	static void drawCell(int x, int y, int id, const Image* image) {
		image->draw(x * 32, y * 32, id * 32, 0, 32, 32);
	}

	// 이동
	void move(int dx, int dy, Type replaced) {
		// 이동 방향을 대입
		mMoveX = dx;
		mMoveY = dy;
		mType = replaced; // 바닥 교체
	}

	Type mType;
	bool mGoalFlag;
	int mMoveX;
	int mMoveY;
};



State::State(const char* stageData, int size)
	: mImage(0), mMoveCount(0), mStageData(0), 
	  mStageDataSize(size), mWidth(0), mHeight(0)
{
	// 처음 맵 정보를 읽는 변수
	mStageData = new char[size + 1];
	for (int i = 0; i < size; ++i) 
	{
		mStageData[i] = stageData[i];
	}
	reset();
	mImage = new Image("./data/image/nimotsuKunImage2.dds");
}

State::~State()
{
	SAFE_DELETE(mImage);
	SAFE_DELETE(mStageData);

}

void State::reset()
{
	setSize();
	mObjects.setSize(mWidth, mHeight);
	int x = 0, y = 0;

	for (int i = 0; i < mStageDataSize; ++i) {
		Object t;

		// 처음 맵 정보로 다시 초기화 한다.
		switch (mStageData[i]) 
		{
		case '#': case ' ': case 'o': case 'O':
		case '.': case 'p': case 'P':
			mObjects(x, y).set(mStageData[i]);
			++x;
			break;
		case '\n': x = 0; ++y; break; //둂뛱룉뿚
		}
	}
}

void State::setSize()
{
	mWidth = mHeight = 0;
	int x = 0;
	int y = 0;
	for (int i = 0; i < mStageDataSize; ++i) {
		switch (mStageData[i]) {
		case '#': case ' ': case 'o': case 'O':
		case '.': case 'p': case 'P':
			++x;
			break;
		case '\n':
			++y;
			//띍묈뭠뛛륷
			mWidth = (mWidth > x) ? mWidth : x;
			mHeight = (mHeight > y) ? mHeight : y;
			x = 0;
			break;
		}
	}
}



void State::draw() const 
{
	for (int y = 0; y < mHeight; ++y) {
		for (int x = 0; x < mWidth; ++x) {
			mObjects(x, y).drawBackground(x, y, mImage);
		}
	}
	for (int y = 0; y < mHeight; ++y) {
		for (int x = 0; x < mWidth; ++x) {
			mObjects(x, y).drawForeGround(x, y, mImage, mMoveCount);
		}
	}
}


void State::update(int dx, int dy) 
{
	// 시간이 0.5초가 지나면
	if (mMoveCount >= MAX_MOVE_COUNT)
	{
		mMoveCount = 0; // 카운트 초기화
		for (int y = 0; y < mHeight; y++)
		{
			for (int x = 0; x < mWidth; x++)
			{
				// 이동 방향 초기화
				mObjects(x, y).mMoveX = 0;
				mObjects(x, y).mMoveY = 0;
			}
		}
	}
	// 이동 중에는 갱신하지 않는다.
	if (mMoveCount > 0)
	{
		++mMoveCount;
		return;
	}

	Array2D<Object>& o = mObjects;

	// 플레이어 위치 찾기
	int x = 0, y = 0;
	bool found = false;
	for (y = 0; y < mHeight; y++)
	{
		for (x = 0; x < mWidth; x++)
		{
			if (o(x,y).mType == Object::OBJ_MAN)
			{
				found = true;
				break;
			}
		}
		if (found) {
			break;
		}
	}

	// 목표 위치
	int tx = x + dx;
	int ty = y + dy;
	// 세상 밖에 나가지 못하게
	if (tx < 0 || ty < 0 || tx >= mWidth || ty >= mHeight) {
		return;
	}

	// 목표 위치가 빈칸이거나 점수 위치
	if (o(tx, ty).mType == Object::OBJ_SPACE) {
		// 목표 위치로 좌표를 옮기고 값을 대입한다.
		o(tx, ty).move(dx, dy, Object::OBJ_MAN);
		// 원래 위치의 값을 복구한다.
		o(x, y).move(dx, dy, Object::OBJ_SPACE);
		mMoveCount = 1; // mMoveCount > 0 조건 이동 시작 					
	}

	// 목표 위치가 상자일 경우 - 목표 위치의 다음칸이 빈칸이거나 점수 위치일 경우 이동
	else if (o(tx, ty).mType == Object::OBJ_BLOCK) {
		// 상자 위치 계산
		int tx2 = tx + dx;
		int ty2 = ty + dy;
		if (tx2 < 0 || ty2 < 0 || tx2 >= mWidth || ty2 >= mHeight) { //상자 위치 유효성 검사
			return;
		}
		if (o(tx2, ty2).mType == Object::OBJ_SPACE) {
			// 차례대로 값을 교체한다
			o(tx2, ty2).move(dx, dy, Object::OBJ_BLOCK);
			o(tx, ty).move(dx, dy, Object::OBJ_MAN);
			o(x, y).move(dx, dy, Object::OBJ_SPACE);
			mMoveCount = 1; // 이동 시작 점화
		}
	}

}


bool State::hasCleared() const 
{
	for (int y = 0; y < mHeight; ++y) {
		for (int x = 0; x < mWidth; ++x) {
			if (mObjects(x, y).mType ==Object::OBJ_BLOCK) {
				if (mObjects(x, y).mGoalFlag == false) {
					return false;
				}
			}
		}
	}
	return true;
}