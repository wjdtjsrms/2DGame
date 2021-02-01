#include "GameLib/GameLib.h"
#include "Sequence/Parent.h"
#include "Sequence/Title.h"
#include "SoundManager.h"

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
		mMode(MODE_NONE),
		mChild(0)
	{
		SoundManager::create();
		mChild = new Title(); // 처음에는 title로 시작한다.
	}

	Parent::~Parent() {
		SAFE_DELETE(mChild);
		SoundManager::destroy();
	}


	void Parent::update() {

		Base* nextChild = mChild->update(this);
		if (nextChild != mChild)
		{
			Child* casted = dynamic_cast<Child*>(nextChild);
			ASSERT(casted);
			SAFE_DELETE(mChild);
			mChild = casted;
		}

		nextChild = 0;
	}

	void Parent::setMode(Mode mode) {
		mMode = mode;
	}

	Parent::Mode Parent::mode() const {
		return mMode;
	}

}