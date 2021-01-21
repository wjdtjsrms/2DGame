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

	// �������� �������� ���ڷ� �ڽ��� �ʱ�ȭ
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
	// ��� ���� ������ �׸���
	void drawBackground(int x, int y, const Image* image) const {
		// �ٴ� �ƴϸ� �� �׸���
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

		// ���� �׸���
		if (mType == OBJ_BLOCK) {
			id = IMAGE_ID_BLOCK;
		}
		else if (mType == OBJ_MAN) {
			id = IMAGE_ID_PLAYER;
		}
		// �����̴� ��ü ǥ��?
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

	// �̵�
	void move(int dx, int dy, Type replaced) {
		// �̵� ������ ����
		mMoveX = dx;
		mMoveY = dy;
		mType = replaced; // �ٴ� ��ü
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
	// ó�� �� ������ �д� ����
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

		// ó�� �� ������ �ٽ� �ʱ�ȭ �Ѵ�.
		switch (mStageData[i]) 
		{
		case '#': case ' ': case 'o': case 'O':
		case '.': case 'p': case 'P':
			mObjects(x, y).set(mStageData[i]);
			++x;
			break;
		case '\n': x = 0; ++y; break; //���s����
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
			//�ő�l�X�V
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
	// �ð��� 0.5�ʰ� ������
	if (mMoveCount >= MAX_MOVE_COUNT)
	{
		mMoveCount = 0; // ī��Ʈ �ʱ�ȭ
		for (int y = 0; y < mHeight; y++)
		{
			for (int x = 0; x < mWidth; x++)
			{
				// �̵� ���� �ʱ�ȭ
				mObjects(x, y).mMoveX = 0;
				mObjects(x, y).mMoveY = 0;
			}
		}
	}
	// �̵� �߿��� �������� �ʴ´�.
	if (mMoveCount > 0)
	{
		++mMoveCount;
		return;
	}

	Array2D<Object>& o = mObjects;

	// �÷��̾� ��ġ ã��
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

	// ��ǥ ��ġ
	int tx = x + dx;
	int ty = y + dy;
	// ���� �ۿ� ������ ���ϰ�
	if (tx < 0 || ty < 0 || tx >= mWidth || ty >= mHeight) {
		return;
	}

	// ��ǥ ��ġ�� ��ĭ�̰ų� ���� ��ġ
	if (o(tx, ty).mType == Object::OBJ_SPACE) {
		// ��ǥ ��ġ�� ��ǥ�� �ű�� ���� �����Ѵ�.
		o(tx, ty).move(dx, dy, Object::OBJ_MAN);
		// ���� ��ġ�� ���� �����Ѵ�.
		o(x, y).move(dx, dy, Object::OBJ_SPACE);
		mMoveCount = 1; // mMoveCount > 0 ���� �̵� ���� 					
	}

	// ��ǥ ��ġ�� ������ ��� - ��ǥ ��ġ�� ����ĭ�� ��ĭ�̰ų� ���� ��ġ�� ��� �̵�
	else if (o(tx, ty).mType == Object::OBJ_BLOCK) {
		// ���� ��ġ ���
		int tx2 = tx + dx;
		int ty2 = ty + dy;
		if (tx2 < 0 || ty2 < 0 || tx2 >= mWidth || ty2 >= mHeight) { //���� ��ġ ��ȿ�� �˻�
			return;
		}
		if (o(tx2, ty2).mType == Object::OBJ_SPACE) {
			// ���ʴ�� ���� ��ü�Ѵ�
			o(tx2, ty2).move(dx, dy, Object::OBJ_BLOCK);
			o(tx, ty).move(dx, dy, Object::OBJ_MAN);
			o(x, y).move(dx, dy, Object::OBJ_SPACE);
			mMoveCount = 1; // �̵� ���� ��ȭ
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