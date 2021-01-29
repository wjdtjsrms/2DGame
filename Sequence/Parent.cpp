#include "GameLib/GameLib.h"
#include "Sequence/Parent.h"
#include "Sequence/Game/GameParent.h"
#include "Sequence/GameOver.h"
#include "Sequence/Ending.h"
#include "Sequence/Title.h"

namespace Sequence
{
	// 싱글톤 초기화
	Parent* Parent::mInstance = 0;

	void Parent::create() {
		ASSERT(!mInstance);
		mInstance = new Parent();
	}

	void Parent::destroy() {
		ASSERT(mInstance);
		SAFE_DELETE(mInstance);
	}

	Parent* Parent::instance() {
		return mInstance;
	}


	Parent::Parent() :
		mNextSequence(NEXT_NONE),
		mMode(MODE_NONE),
		mChild(0)
	{
		mChild = new Title(); // 처음에는 title로 시작한다.
	}

	Parent::~Parent() {
		SAFE_DELETE(mChild);
	}


	void Parent::update() {
		// 가상 함수를 모르고 살다니 인생 절반 손해 봤어
		mChild->update(this);

		switch (mNextSequence) {
		case NEXT_TITLE:
			SAFE_DELETE(mChild);
			mChild = new Title();
			break;
		case NEXT_GAME:
			SAFE_DELETE(mChild);
			mChild = new Game::Parent(mMode);
			break;
		case NEXT_GAME_OVER:
			SAFE_DELETE(mChild);
			mChild = new GameOver();
			break;
		case NEXT_ENDING:
			SAFE_DELETE(mChild);
			mChild = new Ending();
		}

		mNextSequence = NEXT_NONE; // 현재 상태 유지
	}

	void Parent::moveTo(NextSequence next) {
		ASSERT(mNextSequence == NEXT_NONE); 
		mNextSequence = next;
	}

	void Parent::setMode(Mode mode) {
		mMode = mode;
	}

	Parent::Mode Parent::mode() const {
		return mMode;
	}

}