#include "GameLib/GameLib.h"
#include "Sequence/Parent.h"
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
		Child* nextChild = mChild->update(this);
		if (nextChild != mChild)
		{
			SAFE_DELETE(mChild);
			mChild = nextChild;
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