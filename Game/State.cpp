#include "GameLib\Framework.h"

#include "Game\State.h"
#include "Game\StaticObject.h"
#include "Game\DynamicObject.h"

#include "Image.h"

using namespace std;
using namespace GameLib;

namespace // ���� ���� ��ȿ����
{
	// �� ������
	const int WIDTH = 19;
	const int HEIGHT = 15;


	struct StageData {
		int mEnemyNumber; // ���� ��
		int mBrickRate; // ���� ����(�ۼ�Ʈ)
		int mItemPowerNumber; // ��ǳ �������� ��
		int mItemBombNumber; // ��ź �������� ��
	};
	
	// �� ���������� ���� ��
	StageData gStageData[] = {
	{ 2, 90, 4, 6, },
	{ 3, 80, 1, 0, },
	{ 6, 30, 0, 1, },
	};
}

State::~State()
{
	SAFE_DELETE(mImage);
	SAFE_DELETE_ARRAY(mDynamicObjects);
}

State::State(int stageID)
	:mImage(0),mStageID(stageID), mDynamicObjects(0),mDynamicObjectNumber(0)
{
	Framework f = Framework::instance(); 
	mStaticObjects.setSize(WIDTH, HEIGHT);

	mImage = new Image("data/image/bakudanBitoImage.dds");

	const StageData& stageData = gStageData[mStageID];
	int n = WIDTH * HEIGHT;

	// ���� ���� ����� ����
	unsigned* brickList = new unsigned[n];
	int brickNumber = 0;

	// �ٴ� ���� ����� ����
	unsigned* floorList = new unsigned[n];
	int floorNumber = 0;

	for (int y = 0; y < HEIGHT; ++y)
	{
		for (int x = 0; x < WIDTH; ++x)
		{
			StaticObject& o = mStaticObjects(x, y); // ������

			// �׵θ��� ��ũ��Ʈ�� ��ġ�Ѵ�.
			if (x == 0 || y == 0 || (x == WIDTH - 1) || (y == HEIGHT - 1)) 
			{
				o.setFlag(StaticObject::FLAG_WALL);
			}
			// 2�� ������� ��ũ��Ʈ�� ��ġ�Ѵ�.
			else if ((x % 2 == 0) && (y % 2 == 0)) 
			{
				o.setFlag(StaticObject::FLAG_WALL);

			}
			else if (y + x < 4)
			{
				// ���� �� 3ĭ�� �ٴ�
			}
			else if ((stageID == 0) && (y + x > (WIDTH + HEIGHT - 6)))
			{
				// 2�ο��� ��� ������ �Ʒ� 3ĭ�� �ٴ�
			}
			else // �������� �����̳� �ٴ�
			{
				if (f.getRandom(100) < stageData.mBrickRate)
				{
					o.setFlag(StaticObject::FLAG_BRICK);
					// 32��Ʈ ������ 16��Ʈ ���� �ΰ��� �����Ѵ�.
					brickList[brickNumber] = (x << 16) + y;
					++brickNumber;
				}
				else
				{
					floorList[floorNumber] = (x << 16) + y;
					++floorNumber;
				}
			}
		}
	}

	// �������� ������ ����Ѵ�.
	int powerNumber = stageData.mItemPowerNumber;
	int bombNumber = stageData.mItemBombNumber;

	// ���� ����Ʈ i ��°�� ������ �Ͱ� ��ȯ�� �װ��� �������� �ִ´�.
	for (int i = 0; i < powerNumber + bombNumber; ++i)
	{
		// swapped�� ������ brick �� �� �������� ����� �ʰ� �Ѵ�.
		int swapped = f.getRandom(brickNumber - 1 - i) + i;
		unsigned t = brickList[i];  // ���� swap �Ѵ�.
		brickList[i] = brickList[swapped];
		brickList[swapped] = t;

		int x = brickList[i] >> 16;
		int y = brickList[i] & 0xffff;

		StaticObject& o = mStaticObjects(x, y);
		if (i < powerNumber)
		{
			o.setFlag(StaticObject::FLAG_ITEM_POWER);
		}
		else
		{
			o.setFlag(StaticObject::FLAG_ITEM_BOMB);
		}
	}
	SAFE_DELETE_ARRAY(brickList);

	// 2p ���� 1p���� ����
	int playerNumber = (mStageID == 0) ? 2 : 1;
	int enemyNumber = stageData.mEnemyNumber;
	mDynamicObjectNumber = playerNumber + enemyNumber;
	mDynamicObjects = new DynamicObject[mDynamicObjectNumber];

	// 1p ��ġ ���� 
	mDynamicObjects[0].set(1, 1, DynamicObject::TYPE_1P);
	if (mStageID == 0)
	{
		// 2p ��ġ ���� 
		mDynamicObjects[1].set(WIDTH - 2, HEIGHT - 2, DynamicObject::TYPE_2P);
	}

	for (int i = 0; i < enemyNumber; ++i)
	{
		int swapped = f.getRandom(floorNumber - 1 - i) + i;
		unsigned t = floorList[i];
		floorList[i] = floorList[swapped];
		floorList[swapped] = t;

		int x = floorList[i] >> 16;
		int y = floorList[i] & 0xffff;
		mDynamicObjects[playerNumber + i].set(x,y,DynamicObject::TYPE_ENEMY);

	}
	SAFE_DELETE_ARRAY(floorList);
}

void State::draw() const 
{
	// ��, �ٴ�, ������
	for (int y = 0; y < HEIGHT; ++y) {
		for (int x = 0; x < WIDTH; ++x) {
			mStaticObjects(x, y).draw(x, y, mImage);
		}
	}
	// �÷��̾�, ��
	for (int i = 0; i < mDynamicObjectNumber; ++i)
	{
		mDynamicObjects[i].draw(mImage);
	}

	//TODO: ��ǳ �׸���
}

void State::update() {
	for (int i = 0; i < mDynamicObjectNumber; ++i)
	{
		mDynamicObjects[i].update();
	}
}

bool State::hasCleared() const 
{
	//TODO:
	return false;
}


bool State::isAlive1P() const {
	//TODO:
	return true;
}

bool State::isAlive2P() const {
	//TODO:
	return true;
}
