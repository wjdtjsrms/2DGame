#include "GameLib\GameLib.h"
using namespace GameLib;

#include "GameLib\Framework.h"
#include "Sequence\StageSelect.h"
#include "Sequence\Parent.h"
#include "Image.h"
#include "StringRenderer.h"
#include "sstream"
using namespace std;

namespace Sequence {
	StageSelect::StageSelect() 
		:mImage(0), mCursorPosition(0)
	{
		mImage = new Image("data/image/dummy.dds");
	}

	StageSelect::~StageSelect()
	{
		SAFE_DELETE(mImage);
	}

	void StageSelect::update(Parent* parent)
	{
		Framework f = Framework::instance();

		if (f.isKeyTriggered('w')) {
			--mCursorPosition;
			if (mCursorPosition < 1 - 1) { // 순환 범위
				mCursorPosition = 9 - 1;
			}
		}
		else if (f.isKeyTriggered('z')) {
			++mCursorPosition;
			if (mCursorPosition > 9 - 1) { // 순환 범위
				mCursorPosition = 1 - 1;
			}
		}
		else if (f.isKeyTriggered(' ')) {
			parent->setStageID(mCursorPosition + 1); // 선택한 스테이지로 설정
			parent->moveTo(Parent::SEQ_GAME);
		}
		mImage->draw();

		f.drawDebugString(0,0,"STAGE SELECTION");

		ostringstream oss;
		for (int i = 1; i <= 9; ++i) {
			oss << i;
			f.drawDebugString(1, i, oss.str().c_str());
			oss.str(""); // 초기화
		}
		f.drawDebugString(0, mCursorPosition+1, ">");
	}
}


