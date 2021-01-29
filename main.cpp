#include "GameLib/Framework.h"
using namespace GameLib;
#include "Sequence/Parent.h"
#include "Game\KeyBoard.h"

namespace GameLib {
	void Framework::update() {

		if (!Sequence::Parent::instance()) {
			Sequence::Parent::create();
			setFrameRate(60); // �������� 60���� �����
		}

		Sequence::Parent::instance()->update();

		// ������ Ű
		if (KeyBoard::isTriggered(KeyBoard::QUIT)) {
			requestEnd();
		}
		// ���� �Լ�
		if (isEndRequested()) {
			Sequence::Parent::destroy();
		}
	}
}








