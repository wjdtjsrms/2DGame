#include "GameLib/GameLib.h"
#include "Sequence/Parent.h"

#include "Sequence\Game\GameParent.h"
#include "Sequence/Game/Loading.h"
#include "Sequence/Game/Clear.h"
#include "Sequence/Game/Menu.h"
#include "Sequence/Game/Play.h"
#include "Sequence\Game\Failure.h"
#include "Sequence\Game\Judge.h"
#include "Sequence\Game\Pause.h"
#include "Sequence\Game\Ready.h"

#include "Game\State.h"
#include "File.h"
#include <sstream>

namespace Sequence {
	namespace Game {

		Parent::Parent(GrandParent::Mode mode) :
			mState(0),
			mStageID(0), // 1 스테이지에서 시작
			mLife(INITIALI_LIFE_NUMBER),
			mNextSequence(NEXT_NONE),
			mChild(0)
		{
			if (mode == GrandParent::MODE_1P) {
				mStageID = 1;
			}
			else {
				mStageID = 0;
			}
			mChild = new Ready();
		}

		Parent::~Parent() {
			SAFE_DELETE(mState);
			SAFE_DELETE(mChild);
		}

		void Parent::update(GrandParent* parent) {

			mChild->update(this);

			// 시퀸스 변경
			switch (mNextSequence) {
			case NEXT_CLEAR:
				SAFE_DELETE(mChild);
				mChild = new Clear();
				++mStageID; // 다음 스테이지로
				break;
			case NEXT_READY:
				SAFE_DELETE(mChild);
				mChild = new Ready();
				break;
			case NEXT_PAUSE:
				SAFE_DELETE(mChild);
				mChild = new Pause();
				break;
			case NEXT_PLAY:
				SAFE_DELETE(mChild);
				mChild = new Play();
				break;
			case NEXT_FAILURE:
				SAFE_DELETE(mChild);
				mChild = new Failure();
				--mLife; // 목숨 하나 차감
				break;
			case NEXT_JUDGE:
				SAFE_DELETE(mChild);
				mChild = new Judge();
				break;
			case NEXT_ENDING:
				SAFE_DELETE(mChild);
				parent->moveTo(GrandParent::NEXT_ENDING);
				break;
			case NEXT_GAME_OVER:
				SAFE_DELETE(mChild);
				parent->moveTo(GrandParent::NEXT_GAME_OVER);
				break;
			case NEXT_TITLE:
				SAFE_DELETE(mChild);
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
			mState = new State(mStageID);
		}

		void Parent::drawState() const {
			mState->draw();
		}

		Parent::PlayerID Parent::winner() const {
			return mWinner;
		}

		void Parent::setWinner(PlayerID id) {
			mWinner = id;
		}
	}

}