#include "GameLib/GameLib.h"
#include "GameLib/Framework.h"
using namespace GameLib;

#include "Sequence\Title.h"
#include "Sequence/GameOver.h"
#include "Sequence/Parent.h"
#include "Image.h"

namespace Sequence {

	GameOver::GameOver() : mImage(0), mCount(0) {
		mImage = new Image("data/image/dummy.dds");
	}

	GameOver::~GameOver() {
		SAFE_DELETE(mImage);
	}

	// 1초 대기 후 타이틀로
	Child* GameOver::update(Parent* ) {
		Child* next = this;
		if (mCount == 60)
		{
			next = new Title;
		}
		
		Framework::instance().drawDebugString(0, 0, "GAME OVER");
		mImage->draw();
		++mCount;

		return next;
	}

} 