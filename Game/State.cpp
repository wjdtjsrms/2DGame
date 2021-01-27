#include "GameLib\Framework.h"

#include "Game\State.h"
#include "Game\StaticObject.h"
#include "Game\DynamicObject.h"

#include "Image.h"

using namespace std;
using namespace GameLib;

namespace // 번역 단위 유효범위
{
	// 맵 사이즈
	const int WIDTH = 19;
	const int HEIGHT = 15;

	// 폭발 시간과 폭탄 유지 시간
	static const int EXPLOSION_TIME = 120; //3초
	static const int EXPLOSION_LIFE = 30; //1초

	struct StageData {
		int mEnemyNumber; // 적의 수
		int mBrickRate; // 벽돌 비율(퍼센트)
		int mItemPowerNumber; // 폭풍 아이템의 수
		int mItemBombNumber; // 폭탄 아이템의 수
	};

	// 각 스테이지의 설정 값
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

	// 벽돌 블럭을 기록할 영역
	unsigned* brickList = new unsigned[n];
	int brickNumber = 0;

	// 바닥 블럭을 기록할 영역
	unsigned* floorList = new unsigned[n];
	int floorNumber = 0;

	for (int y = 0; y < HEIGHT; ++y)
	{
		for (int x = 0; x < WIDTH; ++x)
		{
			StaticObject& o = mStaticObjects(x, y); // 참조자

			// 테두리에 콘크리트를 배치한다.
			if (x == 0 || y == 0 || (x == WIDTH - 1) || (y == HEIGHT - 1))
			{
				o.setFlag(StaticObject::FLAG_WALL);
			}
			// 2의 배수마다 콘크리트를 배치한다.
			else if ((x % 2 == 0) && (y % 2 == 0))
			{
				o.setFlag(StaticObject::FLAG_WALL);

			}
			else if (y + x < 4)
			{
				// 왼쪽 위 3칸은 바닥
			}
			else if ((stageID == 0) && (y + x > (WIDTH + HEIGHT - 6)))
			{
				// 2인용의 경우 오른쪽 아래 3칸도 바닥
			}
			else // 나머지는 벽돌이나 바닥
			{
				if (f.getRandom(100) < stageData.mBrickRate)
				{
					o.setFlag(StaticObject::FLAG_BRICK);
					// 32비트 변수에 16비트 변수 두개를 저장한다.
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

	// 아이템의 갯수를 기록한다.
	int powerNumber = stageData.mItemPowerNumber;
	int bombNumber = stageData.mItemBombNumber;

	// 벽돌 리스트 i 번째를 적당한 것과 교환해 그곳에 아이템을 넣는다.
	for (int i = 0; i < powerNumber + bombNumber; ++i)
	{
		// swapped의 범위가 brick 의 총 갯수에서 벗어나지 않게 한다.
		int swapped = f.getRandom(brickNumber - 1 - i) + i;
		unsigned t = brickList[i];  // 서로 swap 한다.
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

	// 2p 인지 1p인지 구분
	int playerNumber = (mStageID == 0) ? 2 : 1;
	int enemyNumber = stageData.mEnemyNumber;
	mDynamicObjectNumber = playerNumber + enemyNumber;
	mDynamicObjects = new DynamicObject[mDynamicObjectNumber];

	// 1p 위치 지정 
	mDynamicObjects[0].set(1, 1, DynamicObject::TYPE_PLAYER);
	mDynamicObjects[0].mPlayerID = 0; // 생존 신고할때 사용하는 정보
	if (mStageID == 0)
	{
		// 2p 위치 지정 
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
	// 폭탄 처리
	for (int y = 0; y < HEIGHT; ++y)
	{
		for (int x = 0; x < WIDTH; ++x)
		{
			StaticObject& o = mStaticObjects(x, y);
			if (o.checkFlag(StaticObject::FLAG_BOMB)) // 아직 안 터진 폭탄일때
			{
				++o.mCount; // 폭탄 시간을 잰다.

				if (o.checkFlag(StaticObject::FLAG_EXPLODING)) // 터지고 있는 폭탄
				{
					if (o.mCount == EXPLOSION_LIFE) // 폭발 지속 시간이 다되면
					{
						// 폭탄이랑 폭풍을 없앤다.
						o.resetFlag(StaticObject::FLAG_EXPLODING | StaticObject::FLAG_BOMB);
						o.mCount = 0;
					}
				}
				else // 아직도 안 터진 폭탄일때
				{
					if (o.mCount == EXPLOSION_TIME) // 터질 시간이 되면
					{
						o.setFlag(StaticObject::FLAG_EXPLODING); // 터지고 있는 폭탄으로 변경
						o.mCount = 0;
					}
					else if (o.checkFlag(StaticObject::FLAG_FIRE_X | StaticObject::FLAG_FIRE_Y))
					{
						o.setFlag(StaticObject::FLAG_EXPLODING); // 연쇄 폭발이 일어난다.
						o.mCount = 0;
					}
				}
			}
			else if (o.checkFlag(StaticObject::FLAG_BRICK))
			{
				if (o.checkFlag(StaticObject::FLAG_FIRE_X | StaticObject::FLAG_FIRE_Y)) // 폭발 범위 안에 있는지
				{
					++o.mCount;
					if (o.mCount == EXPLOSION_LIFE)
					{
						o.mCount = 0;
						o.resetFlag(StaticObject::FLAG_BRICK); // 블럭 삭제
					}
				}
			}

			o.resetFlag(StaticObject::FLAG_FIRE_X | StaticObject::FLAG_FIRE_Y);
		}
	}
	// 폭탄 폭풍 처리
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

	// 1p 와 2p의 폭탄 갯수 기록
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

		if (o.isDead()) // 죽었으면 빠른 무시
		{
			continue;
		}

		// 폭탄에게 한번이라도 벗어날때까지 폭탄과의 충돌 검출 무시
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
		o.getCell(&x, &y); // 현재 오브젝트의 배열 위치 반환	
		int wallsX[9];
		int wallsY[9];
		int wallNumber = 0;

		// 플레이어 주변 9칸 판정
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
						// 벽의 좌표값 기록
						wallsX[wallNumber] = x + i - 1;
						wallsY[wallNumber] = y + j - 1;
						++wallNumber;
					}
				}

			}
		}

		// 충돌 처리 후 움직인다.
		o.move(wallsX, wallsY, wallNumber);

		// 플레이어 주변 9칸 판정
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				StaticObject& so = mStaticObjects(x + i - 1, y + j - 1);
				if (o.isIntersectWall(x + i - 1, y + j - 1)) // 플레이어 주변 9칸중 무언가 닿으면
				{
					if (so.checkFlag(StaticObject::FLAG_FIRE_X | StaticObject::FLAG_FIRE_Y))
					{
						o.die(); // 닿은게 폭풍이면 죽는다.
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

		o.getCell(&x, &y); // 현재 오브젝트의 배열 위치 반환
		if (o.hasBombButtonPressed()) // 폭탄 설치 버튼을 누르면
		{
			if (bombNumber[o.mPlayerID] < o.mBombNumber)
			{
				StaticObject&  so = mStaticObjects(x, y);
				if (!so.checkFlag(StaticObject::FLAG_BOMB))
				{
					so.setFlag(StaticObject::FLAG_BOMB); // 현재 위치에 폭탄 설치
					so.mBombOwner = &o;
					so.mCount = 0; // 폭탄 타이머 설정

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


	// player와 enemy의 충돌 처리
	for (int i = 0; i < mDynamicObjectNumber; ++i) {
		for (int j = i + 1; j < mDynamicObjectNumber; ++j) {
			mDynamicObjects[i].doCollisionReactionToDynamic(&mDynamicObjects[j]);
		}
	}

}

void State::draw() const
{
	// 벽, 바닥, 아이템
	for (int y = 0; y < HEIGHT; ++y) {
		for (int x = 0; x < WIDTH; ++x) {
			mStaticObjects(x, y).draw(x, y, mImage);
		}
	}
	// 플레이어, 적
	for (int i = 0; i < mDynamicObjectNumber; ++i)
	{
		mDynamicObjects[i].draw(mImage);
	}

	// 폭풍 그리기
	for (int y = 0; y < HEIGHT; ++y) {
		for (int x = 0; x < WIDTH; ++x) {
			mStaticObjects(x, y).drawExplosion(x, y, mImage);
		}
	}
}

void State::setFire(int x, int y)
{
	// x와 y는 폭풍 시작 위치
	StaticObject& o = mStaticObjects(x, y);
	int power = o.mBombOwner->mBombPower;
	int end = 0;


	// x축 왼쪽 폭발 처리
	end = (x - power < 0) ? 0 : (x - power);
	for (int i = x - 1; i >= end; --i)
	{
		StaticObject& to = mStaticObjects(i, y);
		to.setFlag(StaticObject::FLAG_FIRE_X); // 바닥을 폭발 x로 바꿈
		if (to.checkFlag(StaticObject::FLAG_WALL | StaticObject::FLAG_BRICK | StaticObject::FLAG_BOMB))
		{
			// 벽,콘크리트,폭탄을 만나면 그만 터진다.
			break;
		}
		else
		{
			// 폭발 범위에 아이템이 있으면 삭제
			to.resetFlag(StaticObject::FLAG_ITEM_BOMB | StaticObject::FLAG_ITEM_POWER);
		}
	}

	// x축 오른쪽 폭발 처리
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
	// y축 위쪽 폭발 처리
	end = (y - power < 0) ? 0 : (y - power);
	for (int i = y - 1; i >= end; --i)
	{
		StaticObject& to = mStaticObjects(x, i);
		to.setFlag(StaticObject::FLAG_FIRE_Y); // 바닥을 폭발 y로 바꿈
		if (to.checkFlag(StaticObject::FLAG_WALL | StaticObject::FLAG_BRICK | StaticObject::FLAG_BOMB))
		{
			break;
		}
		else
		{
			to.resetFlag(StaticObject::FLAG_ITEM_BOMB | StaticObject::FLAG_ITEM_POWER);
		}
	}

	// y축 아래쪽 폭발 처리
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
			if ((o.mCount == 0) && to.checkFlag(StaticObject::FLAG_BRICK))  // 시간 동기화?
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

