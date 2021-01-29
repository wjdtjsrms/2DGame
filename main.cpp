#include "GameLib/Framework.h"
using namespace GameLib;
#include "Sequence/Parent.h"
#include "Game\KeyBoard.h"

namespace GameLib {
	void Framework::update() {

		if (!Sequence::Parent::instance()) {
			Sequence::Parent::create();
			setFrameRate(60); // 프레임을 60으로 맞춘다
		}

		Sequence::Parent::instance()->update();

		// 나가기 키
		if (KeyBoard::isTriggered(KeyBoard::QUIT)) {
			requestEnd();
		}
		// 종료 함수
		if (isEndRequested()) {
			Sequence::Parent::destroy();
		}
	}
}








