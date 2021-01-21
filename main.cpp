#include "GameLib/Framework.h"
using namespace GameLib;

#include "State.h"
#include "File.h"
#include "Image.h"
#include "Sequence/Parent.h"

Sequence::Parent* gRootSequence = 0;
int gCounter = 0;


namespace GameLib {
	void Framework::update() {

		if (!gRootSequence) {
			gRootSequence = new Sequence::Parent();
		}

		setFrameRate(60); // �������� 60���� �����ش�.

		if (gCounter % 60 == 0) { // �ֱ� 60�������� ��� ������ ����Ʈ�� �����ش�.
			cout << " FrameRate:" << frameRate() << endl;
		}
		++gCounter;

		gRootSequence->update();

		// ������ Ű
		if (isKeyOn('q')) {
			requestEnd();
		}
		// ���� �Լ�
		if (isEndRequested()) {
			SAFE_DELETE(gRootSequence);
		}
	}
}








