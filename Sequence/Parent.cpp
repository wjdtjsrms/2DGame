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
		mTitle(0),
		mGame(0),
		mGameOver(0),
		mEnding(0),
		mNextSequence(NEXT_NONE),
		mMode(MODE_NONE)
	{
		mTitle = new Title();
	}

	Parent::~Parent() {
		SAFE_DELETE(mTitle);
		SAFE_DELETE(mGame);
		SAFE_DELETE(mGameOver);
		SAFE_DELETE(mEnding);
	}


	void Parent::update() {

		if (mTitle) {
			mTitle->update(this);
		}
		else if (mGame) {
			mGame->update(this);
		}
		else if (mGameOver) {
			mGameOver->update(this);
		}
		else if (mEnding) {
			mEnding->update(this);
		}
		else {
			HALT("BABO!"); // 버그 발생
		}


		switch (mNextSequence) {
		case NEXT_TITLE:
			ASSERT(!mTitle && (mGame || mGameOver || mEnding)); // false 일시 실행
			SAFE_DELETE(mGame);
			SAFE_DELETE(mGameOver);
			SAFE_DELETE(mEnding);
			mTitle = new Title();
			break;
		case NEXT_GAME:
			ASSERT(mTitle && !mGameOver && !mGame && !mEnding); 
			SAFE_DELETE(mTitle);
			mGame = new Game::Parent();
			break;
		case NEXT_GAME_OVER:
			ASSERT(mGame && !mGameOver && !mEnding && !mTitle);
			SAFE_DELETE(mGame);
			mGameOver = new GameOver();
			break;
		case NEXT_ENDING:
			ASSERT(mGame && !mGameOver && !mEnding && !mTitle);
			SAFE_DELETE(mGame);
			mEnding = new Ending();
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