#include "GameLib/GameLib.h"
#include "GameLib/Framework.h"
using namespace GameLib;

#include "Sequence/Game/Loading.h"
#include "Sequence\Game\GameParent.h"
#include "Image.h"
#include "State.h"

namespace Sequence {
	namespace Game {
		Loading::Loading() :
			mImage(0),
			mCount(0),
			mStarted(false) {
			mImage = new Image("data/image/dummy.dds");
		}

		Loading::~Loading() {
			SAFE_DELETE(mImage);
		}

		void Loading::update(Parent* parent) {
			// �ε����� �ʾҴٸ� ȣ���Ѵ�.
			if (!mStarted) {
				parent->startLoading();
				mStarted = true;
			}
			else if (mCount == 60) { //1�� ���
				parent->moveTo(Parent::SEQ_PLAY); // Play ���·� ����
			}

			// �ε� �̹����� �׸���
			mImage->draw();
			Framework::instance().drawDebugString(0, 0, "NOW LOADING~~");
			++mCount;
		}
	}
}