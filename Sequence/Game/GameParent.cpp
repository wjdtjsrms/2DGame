#include "GameLib/GameLib.h"
#include "Sequence/Parent.h"

#include "Sequence\Game\GameParent.h"
#include "Sequence\Game\Ready.h"

#include "Sequence\Ending.h"
#include "Sequence\Title.h"
#include "Sequence\GameOver.h"

#include "Game\State.h"
#include "File.h"
#include <sstream>

namespace Sequence {
	namespace Game {

		Parent::Parent(GrandParent::Mode mode) :
			mState(0),
			mStageID(0),
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

		Base* Parent::update(GrandParent*) {

			Base* next = this; // ���� ���� ó��
			Base* nextChild = mChild->update(this); // ���� ���� ó��

			if (nextChild != mChild) // �������� ���� �Ǿ��ٸ�
			{
				Game::Child* casted = dynamic_cast<Game::Child*>(nextChild); 
				if (casted) // nextChild�� mChild�� ���� �����̶��
				{
					SAFE_DELETE(mChild);
					mChild = casted;
				}
				else
				{
					next = nextChild; // ���� �������ٰ� ����� ��ü ��ȯ
				}
				casted = 0;
			}
			nextChild = 0;

			return next;
		}

		// �ڽ� ��ü���� �� ��� ������ �����Ѵ�.
		void Parent::moveTo(NextSequence next) {
			ASSERT(mNextSequence == NEXT_NONE);
			mNextSequence = next;
		}

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

		void Parent::reduceLife()
		{
			--mLife;
		}
		void Parent::goToNextStage()
		{
			++mStageID;
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
		// state ��ü�� �����Ѵ�.
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