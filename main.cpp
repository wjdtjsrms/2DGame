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

		setFrameRate(60); // 프레임을 60으로 맞춰준다.

		if (gCounter % 60 == 0) { // 최근 60프레임의 평균 프레임 레이트를 보여준다.
			cout << " FrameRate:" << frameRate() << endl;
		}
		++gCounter;

		gRootSequence->update();

		// 나가기 키
		if (isKeyOn('q')) {
			requestEnd();
		}
		// 종료 함수
		if (isEndRequested()) {
			SAFE_DELETE(gRootSequence);
		}
	}
}








