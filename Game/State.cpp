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

	// ���� �ð��� ��ź ���� �ð�
	static const int EXPLOSION_TIME = 120; //3��
	static const int EXPLOSION_LIFE = 30; //1��

	struct StageData {
		int mEnemyNumber; // ���� ��
		int mBrickRate; // ���� ����(�ۼ�Ʈ)
		int mItemPowerNumber; // ��ǳ �������� ��
		int mItemBombNumber; // ��ź �������� ��
	};

	// �� ���������� ���� ��
	StageData gStageData[] = {
	{ 5, 50, 10, 10, },
	{ 5, 40, 10, 10, },
	{ 5, 30, 0, 1, },
	};
}

State::~State()
{
	SAFE_DELETE(mImage);
	SAFE_DELETE_ARRAY(mDynamicObjects);
}

State::State(int stageID)
	:mImage(0), mStageID(stageID), mDynamicObjects(0), mDynamicObjectNumber(0)
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
	mDynamicObjects[0].set(1, 1, DynamicObject::TYPE_PLAYER);
	mDynamicObjects[0].mPlayerID = 0; // ���� �Ű��Ҷ� ����ϴ� ����
	if (mStageID == 0)
	{
		// 2p ��ġ ���� 
		mDynamicObjects[1].set(WIDTH - 2, HEIGHT - 2, DynamicObject::TYPE_PLAYER);
		mDynamicObjects[1].mPlayerID = 1;
	}

	for (int i = 0; i < enemyNumber; ++i)
	{
		int swapped = f.getRandom(floorNumber - 1 - i) + i;
		unsigned t = floorList[i];
		floorList[i] = floorList[swapped];
		floorList[swapped] = t;

		int x = floorList[i] >> 16;
		int y = floorList[i] & 0xffff;
		mDynamicObjects[playerNumber + i].set(x, y, DynamicObject::TYPE_ENEMY);

	}
	SAFE_DELETE_ARRAY(floorList);
}

void State::update() {
	// ��ź ó��
	for (int y = 0; y < HEIGHT; ++y)
	{
		for (int x = 0; x < WIDTH; ++x)
		{
			StaticObject& o = mStaticObjects(x, y);
			if (o.checkFlag(StaticObject::FLAG_BOMB)) // ���� �� ���� ��ź�϶�
			{
				++o.mCount; // ��ź �ð��� ���.

				if (o.checkFlag(StaticObject::FLAG_EXPLODING)) // ������ �ִ� ��ź
				{
					if (o.mCount == EXPLOSION_LIFE) // ���� ���� �ð��� �ٵǸ�
					{
						// ��ź�̶� ��ǳ�� ���ش�.
						o.resetFlag(StaticObject::FLAG_EXPLODING | StaticObject::FLAG_BOMB);
						o.mCount = 0;
					}
				}
				else // ������ �� ���� ��ź�϶�
				{
					if (o.mCount == EXPLOSION_TIME) // ���� �ð��� �Ǹ�
					{
						o.setFlag(StaticObject::FLAG_EXPLODING); // ������ �ִ� ��ź���� ����
						o.mCount = 0;
					}
					else if (o.checkFlag(StaticObject::FLAG_FIRE_X | StaticObject::FLAG_FIRE_Y))
					{
						o.setFlag(StaticObject::FLAG_EXPLODING); // ���� ������ �Ͼ��.
						o.mCount = 0;
					}
				}
			}
			else if (o.checkFlag(StaticObject::FLAG_BRICK))
			{
				if (o.checkFlag(StaticObject::FLAG_FIRE_X | StaticObject::FLAG_FIRE_Y)) // ���� ���� �ȿ� �ִ���
				{
					++o.mCount;
					if (o.mCount == EXPLOSION_LIFE)
					{
						o.mCount = 0;
						o.resetFlag(StaticObject::FLAG_BRICK); // �� ����
					}
				}
			}

			o.resetFlag(StaticObject::FLAG_FIRE_X | StaticObject::FLAG_FIRE_Y);
		}
	}
	// ��ź ��ǳ ó��
	for (int y = 0; y < HEIGHT; ++y)
	{
		for (int x = 0; x < WIDTH; ++x)
		{
			if (mStaticObjects(x, y).checkFlag(StaticObject::FLAG_EXPLODING))
			{
				setFire(x, y);
			}
		}
	}

	// 1p �� 2p�� ��ź ���� ���
	int bombNumber[2];
	bombNumber[0] = bombNumber[1] = 0;
	for (int y = 0; y < HEIGHT; ++y)
	{
		for (int x = 0; x < WIDTH; ++x)
		{
			const StaticObject& o = mStaticObjects(x, y);
			if (o.checkFlag(StaticObject::FLAG_BOMB))
			{
				++bombNumber[o.mBombOwner->mPlayerID];
			}
		}
	}

	for (int i = 0; i < mDynamicObjectNumber; ++i)
	{
		DynamicObject& o = mDynamicObjects[i];

		if (o.isDead()) // �׾����� ���� ����
		{
			continue;
		}

		// ��ź���� �ѹ��̶� ��������� ��ź���� �浹 ���� ����
		for (int j = 0; j < 2; ++j)
		{
			if (o.mLastBombX[j] >= 0)
			{
				if (!o.isIntersectWall(o.mLastBombX[j], o.mLastBombY[j]))
				{
					o.mLastBombX[j] = o.mLastBombY[j] = -1;
				}
			}
		}

		int x, y;
		o.getCell(&x, &y); // ���� ������Ʈ�� �迭 ��ġ ��ȯ	
		int wallsX[9];
		int wallsY[9];
		int wallNumber = 0;

		// �÷��̾� �ֺ� 9ĭ ����
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				int tx = x + i - 1; // -1 0 +1
				int ty = y + j - 1;

				const StaticObject& so = mStaticObjects(tx, ty);
				if (so.checkFlag(StaticObject::FLAG_BRICK | StaticObject::FLAG_WALL | StaticObject::FLAG_BOMB))
				{
					bool myBomb0 = (o.mLastBombX[0] == tx) && (o.mLastBombY[0] == ty);
					bool myBomb1 = (o.mLastBombX[1] == tx) && (o.mLastBombY[1] == ty);

					if (!myBomb0 && !myBomb1)
					{
						// ���� ��ǥ�� ���
						wallsX[wallNumber] = x + i - 1;
						wallsY[wallNumber] = y + j - 1;
						++wallNumber;
					}
				}

			}
		}

		// �浹 ó�� �� �����δ�.
		o.move(wallsX, wallsY, wallNumber);

		// �÷��̾� �ֺ� 9ĭ ����
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				StaticObject& so = mStaticObjects(x + i - 1, y + j - 1);
				if (o.isIntersectWall(x + i - 1, y + j - 1)) // �÷��̾� �ֺ� 9ĭ�� ���� ������
				{
					if (so.checkFlag(StaticObject::FLAG_FIRE_X | StaticObject::FLAG_FIRE_Y))
					{
						o.die(); // ������ ��ǳ�̸� �״´�.
					}
					else if (!so.checkFlag(StaticObject::FLAG_BRICK))
					{
						if (so.checkFlag(StaticObject::FLAG_ITEM_BOMB))
						{
							so.resetFlag(StaticObject::FLAG_ITEM_BOMB);
							++o.mBombNumber;
						}
						else if (so.checkFlag(StaticObject::FLAG_ITEM_POWER))
						{
							so.resetFlag(StaticObject::FLAG_ITEM_POWER);
							++o.mBombPower;
						}
					}
				}


			}
		}

		o.getCell(&x, &y); // ���� ������Ʈ�� �迭 ��ġ ��ȯ
		if (o.hasBombButtonPressed()) // ��ź ��ġ ��ư�� ������
		{
			if (bombNumber[o.mPlayerID] < o.mBombNumber)
			{
				StaticObject&  so = mStaticObjects(x, y);
				if (!so.checkFlag(StaticObject::FLAG_BOMB))
				{
					so.setFlag(StaticObject::FLAG_BOMB); // ���� ��ġ�� ��ź ��ġ
					so.mBombOwner = &o;
					so.mCount = 0; // ��ź Ÿ�̸� ����

					if (o.mLastBombX[0] < 0)
					{
						o.mLastBombX[0] = x;
						o.mLastBombY[0] = y;
					}
					else
					{
						o.mLastBombX[1] = x;
						o.mLastBombY[1] = y;
					}
				}
			}
		}
	}


	// player�� enemy�� �浹 ó��
	for (int i = 0; i < mDynamicObjectNumber; ++i) {
		for (int j = i + 1; j < mDynamicObjectNumber; ++j) {
			mDynamicObjects[i].doCollisionReactionToDynamic(&mDynamicObjects[j]);
		}
	}

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

	// ��ǳ �׸���
	for (int y = 0; y < HEIGHT; ++y) {
		for (int x = 0; x < WIDTH; ++x) {
			mStaticObjects(x, y).drawExplosion(x, y, mImage);
		}
	}
}

void State::setFire(int x, int y)
{
	// x�� y�� ��ǳ ���� ��ġ
	StaticObject& o = mStaticObjects(x, y);
	int power = o.mBombOwner->mBombPower;
	int end = 0;


	// x�� ���� ���� ó��
	end = (x - power < 0) ? 0 : (x - power);
	for (int i = x - 1; i >= end; --i)
	{
		StaticObject& to = mStaticObjects(i, y);
		to.setFlag(StaticObject::FLAG_FIRE_X); // �ٴ��� ���� x�� �ٲ�
		if (to.checkFlag(StaticObject::FLAG_WALL | StaticObject::FLAG_BRICK | StaticObject::FLAG_BOMB))
		{
			// ��,��ũ��Ʈ,��ź�� ������ �׸� ������.
			break;
		}
		else
		{
			// ���� ������ �������� ������ ����
			to.resetFlag(StaticObject::FLAG_ITEM_BOMB | StaticObject::FLAG_ITEM_POWER);
		}
	}

	// x�� ������ ���� ó��
	end = (x + power >= WIDTH) ? (WIDTH - 1) : (x + power);
	for (int i = x + 1; i <= end; ++i)
	{
		StaticObject& to = mStaticObjects(i, y);
		to.setFlag(StaticObject::FLAG_FIRE_X);
		if (to.checkFlag(StaticObject::FLAG_WALL | StaticObject::FLAG_BRICK | StaticObject::FLAG_BOMB))
		{
			break;
		}
		else
		{
			to.resetFlag(StaticObject::FLAG_ITEM_BOMB | StaticObject::FLAG_ITEM_POWER);
		}
	}
	// y�� ���� ���� ó��
	end = (y - power < 0) ? 0 : (y - power);
	for (int i = y - 1; i >= end; --i)
	{
		StaticObject& to = mStaticObjects(x, i);
		to.setFlag(StaticObject::FLAG_FIRE_Y); // �ٴ��� ���� y�� �ٲ�
		if (to.checkFlag(StaticObject::FLAG_WALL | StaticObject::FLAG_BRICK | StaticObject::FLAG_BOMB))
		{
			break;
		}
		else
		{
			to.resetFlag(StaticObject::FLAG_ITEM_BOMB | StaticObject::FLAG_ITEM_POWER);
		}
	}

	// y�� �Ʒ��� ���� ó��
	end = (y + power >= HEIGHT) ? (HEIGHT - 1) : (y + power);
	for (int i = y + 1; i <= end; ++i)
	{
		StaticObject& to = mStaticObjects(x, i);
		to.setFlag(StaticObject::FLAG_FIRE_Y);
		if (to.checkFlag(StaticObject::FLAG_WALL | StaticObject::FLAG_BRICK | StaticObject::FLAG_BOMB))
		{
			break;
		}
		else
		{
			to.resetFlag(StaticObject::FLAG_ITEM_BOMB | StaticObject::FLAG_ITEM_POWER);
		}
	}



	end = (x - power < 0) ? 0 : (x - power);
	for (int i = x - 1; i >= end; --i)
	{
		StaticObject& to = mStaticObjects(i, y);
		if (to.checkFlag(StaticObject::FLAG_WALL | StaticObject::FLAG_BRICK))
		{
			if ((o.mCount == 0) && to.checkFlag(StaticObject::FLAG_BRICK))  // �ð� ����ȭ?
			{
				to.mCount = 0;
			}
			break;
		}
	}

	end = (x + power >= WIDTH) ? (WIDTH - 1) : (x + power);
	for (int i = x + 1; i <= end; ++i)
	{
		StaticObject& to = mStaticObjects(i, y);
		if (to.checkFlag(StaticObject::FLAG_WALL | StaticObject::FLAG_BRICK))
		{
			if ((o.mCount == 0) && to.checkFlag(StaticObject::FLAG_BRICK))
			{
				to.mCount = 0;
			}
			break;
		}
	}

	end = (y - power < 0) ? 0 : (y - power);
	for (int i = y - 1; i >= end; --i)
	{
		StaticObject& to = mStaticObjects(x, i);
		if (to.checkFlag(StaticObject::FLAG_WALL | StaticObject::FLAG_BRICK))
		{
			if ((o.mCount == 0) && to.checkFlag(StaticObject::FLAG_BRICK))
			{
				to.mCount = 0;
			}
			break;
		}
	}

	end = (y + power >= HEIGHT) ? (HEIGHT - 1) : (y + power);
	for (int i = y + 1; i <= end; ++i)
	{
		StaticObject& to = mStaticObjects(x, i);
		if (to.checkFlag(StaticObject::FLAG_WALL | StaticObject::FLAG_BRICK))
		{
			if ((o.mCount == 0) && to.checkFlag(StaticObject::FLAG_BRICK))
			{
				to.mCount = 0;
			}
			break;
		}
	}
}


bool State::hasCleared() const
{
	for (int i = 0; i < mDynamicObjectNumber; ++i)
	{
		if (mDynamicObjects[i].isEnemy())
		{
			return false;
		}
	}
	return true;
}

bool State::isAlive(int playerID) const
{
	for (int i = 0; i < mDynamicObjectNumber; ++i)
	{
		if (mDynamicObjects[i].isPlayer())
		{
			if (mDynamicObjects[i].mPlayerID == playerID)
			{
				return true;
			}
		}
	}
	return false;
}

