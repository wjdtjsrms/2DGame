#include "GameLib\Framework.h"

#include "Game\State.h"
#include "Game\StaticObject.h"

#include "Image.h"

using namespace std;
using namespace GameLib;

namespace // 번역 단위 유효범위
{
	// 맵 사이즈
	const int WIDTH = 19;
	const int HEIGHT = 15;


	struct StageData {
		int mEnemyNumber; // 적의 수
		int mBrickRate; // 벽돌 비율(퍼센트)
		int mItemPowerNumber; // 폭풍 아이템의 수
		int mItemBombNumber; // 폭탄 아이템의 수
	};
	
	// 각 스테이지의 설정 값
	StageData gStageData[] = {
	{ 2, 90, 4, 6, },
	{ 3, 80, 1, 0, },
	{ 6, 30, 0, 1, },
	};
}

State::~State()
{
	SAFE_DELETE(mImage);
}

State::State(int stageID)
	:mImage(0),mStageID(0)
{
	Framework f = Framework::instance(); 
	mStaticObjects.setSize(WIDTH, HEIGHT);

	mImage = new Image("data/image/bakudanBitoImage.dds");

	const StageData& stageData = gStageData[mStageID];
	int n = WIDTH * HEIGHT;

	// 벽돌 블럭을 기록할 영역
	unsigned* brickList = new unsigned[n];
	int brickNumber = 0;

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
}

void State::draw() const 
{
	for (int y = 0; y < HEIGHT; ++y) {
		for (int x = 0; x < WIDTH; ++x) {
			mStaticObjects(x, y).draw(x, y, mImage);
		}
	}

	//TODO: 전경 그리기
	//TODO: 폭풍 그리기
}

void State::update() {
	//TODO:
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
