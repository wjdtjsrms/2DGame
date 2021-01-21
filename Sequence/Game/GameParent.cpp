#include "GameLib/GameLib.h"
#include "Sequence\Game\GameParent.h"
#include "Sequence/Game/Loading.h"
#include "Sequence/Game/Clear.h"
#include "Sequence/Game/Menu.h"
#include "Sequence/Game/Play.h"
#include "Sequence/Parent.h"
#include "State.h"
#include "File.h"
#include <sstream>

namespace Sequence {
	namespace Game {

		Parent::Parent(int stageID) :
			mState(0),
			mStageID(stageID),
			mNext(SEQ_NONE),
			mClear(0),
			mLoading(0),
			mMenu(0),
			mPlay(0) {
			mLoading = new Loading();
		}

		Parent::~Parent() {
			SAFE_DELETE(mState);
			SAFE_DELETE(mClear);
			SAFE_DELETE(mLoading);
			SAFE_DELETE(mMenu);
			SAFE_DELETE(mPlay);
		}

		void Parent::update(GrandParent* parent) {
			// ��ü�� �����Ǿ������� ����
			if (mClear) {
				mClear->update(this);
			}
			else if (mLoading) {
				mLoading->update(this);
			}
			else if (mMenu) {
				mMenu->update(this);
			}
			else if (mPlay) {
				mPlay->update(this);
			}
			else {
				HALT("BABO!"); // ��ȿ���� ���� ��
			}

			switch (mNext) {
			case SEQ_STAGE_SELECT:
				parent->moveTo(GrandParent::SEQ_STAGE_SELECT); // ���� Sequence ��ü ȣ��
				break;
			case SEQ_TITLE:
				parent->moveTo(GrandParent::SEQ_TITLE); // ���� Sequence ��ü ȣ��
				break;
			case SEQ_PLAY:
				SAFE_DELETE(mLoading);
				SAFE_DELETE(mMenu);
				mPlay = new Game::Play();
				break;
			case SEQ_CLEAR:
				SAFE_DELETE(mPlay);
				mClear = new Game::Clear();
				break;
			case SEQ_MENU:
				SAFE_DELETE(mPlay);
				mMenu = new Game::Menu();
				break;
			}


			mNext = SEQ_NONE; // ���� ���°� ������ �ʾҴ�.
		}

		void Parent::startLoading() {
			std::ostringstream oss;
			oss << "data/stageData/" << mStageID << ".txt";
			File file(oss.str().c_str()); //��Ʈ���� const char*������ ��ȯ
			mState = new State(file.data(), file.size());
		}

		// �ڽ� ��ü���� �� ��� ������ �����Ѵ�.
		void Parent::moveTo(SeqID next) {
			mNext = next;
		}

		// �׸��� ����� �ִ� state ��ü�� ��ȯ�Ѵ�.
		State* Parent::state(){
			return mState;
		}

	}

	
}