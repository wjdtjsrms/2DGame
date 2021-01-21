#include "GameLib/GameLib.h"
#include "Sequence\Game\GameParent.h"
#include "Sequence/Game/Loading.h"
#include "Sequence/Game/Clear.h"
#include "Sequence/Game/Menu.h"
#include "Sequence/Game/Play.h"
#include "Sequence/Parent.h"


#include "Sequence\Game\Failure.h"
#include "Sequence\Game\Judge.h"
#include "Sequence\Game\Pause.h"

#include "Sequence\Game\Ready.h"

#include "State.h"
#include "File.h"
#include <sstream>

namespace Sequence {
	namespace Game {

		Parent::Parent():
			mState(0),
			mStageID(1), // 1 스테이지에서 시작
			mLife(INITIALI_LIFE_NUMBER),
			mNextSequence(NEXT_NONE),
			mClear(0),
			mReady(0),
			mPause(0),
			mPlay(0),
			mFailure(0),
			mJudge(0)
		{
			mReady = new Ready();
		}



		Parent::~Parent() {

			SAFE_DELETE(mState);
			SAFE_DELETE(mClear);
			SAFE_DELETE(mReady);
			SAFE_DELETE(mPause);
			SAFE_DELETE(mPlay);
			SAFE_DELETE(mFailure);
			SAFE_DELETE(mJudge);
		}

		void Parent::update(GrandParent* parent) {

			// 객체가 생성되었을때만 실행
			if (mClear) {
				mClear->update(this);
			}
			else if (mReady) {
				mReady->update(this);
			}
			else if (mPause) {
				mPause->update(this);
			}
			else if (mPlay) {
				mPlay->update(this);
			}
			else if (mFailure) {
				mFailure->update(this);
			}
			else if (mJudge) {
				mJudge->update(this);
			}
			else {
				HALT("bakana!"); // 유효하지 않은 값
			}

			// 시퀸스 변경
			switch (mNextSequence) {
			case NEXT_CLEAR:
				ASSERT(!mClear && !mReady && !mPause && mPlay && !mFailure && !mJudge);
				SAFE_DELETE(mPlay);
				mClear = new Clear();
				++mStageID; // 다음 스테이지로
				break;
			case NEXT_READY:
				ASSERT(!mReady && !mPause && !mPlay && (mFailure || mClear || mJudge));
				SAFE_DELETE(mFailure);
				SAFE_DELETE(mClear);
				SAFE_DELETE(mJudge);
				mReady = new Ready();
				break;
			case NEXT_PAUSE:
				ASSERT(!mClear && !mReady && !mPause && mPlay && !mFailure && !mJudge);
				SAFE_DELETE(mPlay);
				mPause = new Pause();
				break;
			case NEXT_PLAY:
				ASSERT(!mClear && (mReady || mPause) && !mPlay && !mFailure && !mJudge);
				SAFE_DELETE(mReady);
				SAFE_DELETE(mPause);
				mPlay = new Play();
				break;
			case NEXT_FAILURE:
				ASSERT(!mClear && !mReady && !mPause && mPlay && !mFailure && !mJudge);
				SAFE_DELETE(mPlay);
				mFailure = new Failure();
				--mLife; // 목숨 하나 차감
				break;
			case NEXT_JUDGE:
				ASSERT(!mClear && !mReady && !mPause && mPlay && !mFailure && !mJudge);
				SAFE_DELETE(mPlay);
				mJudge = new Judge();
				break;
			case NEXT_ENDING:
				ASSERT(mClear && !mReady && !mPause && !mPlay && !mFailure && !mJudge);
				SAFE_DELETE(mClear);
				parent->moveTo(GrandParent::NEXT_ENDING);
				break;
			case NEXT_GAME_OVER:
				ASSERT(!mClear && !mReady && !mPause && !mPlay && mFailure && !mJudge);
				SAFE_DELETE(mFailure);
				parent->moveTo(GrandParent::NEXT_GAME_OVER);
				break;
			case NEXT_TITLE:
				ASSERT(!mClear && !mReady && (mPause || mJudge) && !mPlay && !mFailure);
				SAFE_DELETE(mPause);
				SAFE_DELETE(mJudge);
				parent->moveTo(GrandParent::NEXT_TITLE);
				break;
			}
			mNextSequence = NEXT_NONE;
		}


		// 자식 객체들이 이 멤버 변수를 변경한다.
		void Parent::moveTo(NextSequence next) {
			//ASSERT(mNextSequence == NEXT_NONE);
			mNextSequence = next;
		}
		// 그리기 기능이 있는 state 객체를 반환한다.
		State* Parent::state() {
			return mState;
		}

		bool Parent::hasFinalStageCleared() const
		{
			return (mStageID > FINAL_STAGE);
		}

		int Parent::lifeNumber() const
		{
			return mLife;
		}

		Parent::Mode Parent::mode() const
		{
			Mode r = MODE_NONE;
			switch (GrandParent::instance()->mode()) {
			case GrandParent::MODE_1P: r = MODE_1P; break;
			case GrandParent::MODE_2P: r = MODE_2P; break;
			default: ASSERT(false); break;
			}
			return r;
		}

		void Parent::startLoading() {
			SAFE_DELETE(mState);
			std::ostringstream oss;
			if (mode() == MODE_1P) {
				oss << "data/stageData/" << mStageID << ".txt";
			}
			else {
				oss << "data/stageData/9.txt"; // 멀티 플레이 맵
			}
			File file(oss.str().c_str()); //스트링을 const char*형으로 반환
			mState = new State(file.data(), file.size());
		}


	}

	
}