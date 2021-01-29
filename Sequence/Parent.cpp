#include "GameLib/GameLib.h"
#include "Sequence/Parent.h"
#include "Sequence/Game/GameParent.h"
#include "Sequence/GameOver.h"
#include "Sequence/Ending.h"
#include "Sequence/Title.h"

namespace Sequence
{
	// �̱��� �ʱ�ȭ
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
		mChild = new Title(); // ó������ title�� �����Ѵ�.
	}

	Parent::~Parent() {
		SAFE_DELETE(mChild);
	}


	void Parent::update() {
		// ���� �Լ��� �𸣰� ��ٴ� �λ� ���� ���� �þ�
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

		mNextSequence = NEXT_NONE; // ���� ���� ����
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