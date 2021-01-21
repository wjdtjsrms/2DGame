#include "GameLib/GameLib.h"
#include "GameLib/Framework.h"
using namespace GameLib;

#include "Sequence/Game/Ready.h"
#include "Sequence\Game\GameParent.h"
#include "Image.h"
#include "State.h"

namespace Sequence
{
	namespace  Game
	{
		Ready::Ready()
			:mImage(0),mCount(0), mStarted(false)
		{
			mImage = new Image("data/image/dummy.dds");
		}

		Ready::~Ready()
		{
			SAFE_DELETE(mImage);
		}

		void Ready::update(Parent* parent)
		{
			Framework f = Framework::instance();

			if (!mStarted)
			{
				parent->startLoading();
				mStarted = true;
			}

			if (mCount >= 120)
			{
				parent->moveTo(Parent::NEXT_PLAY);
			}
			else if (mCount >= 60)
			{
				f.drawDebugString(0, 1, "GO!!");
			}
			else 
			{
				f.drawDebugString(0, 1, "READY...");
			}

			parent->state()->draw();
			mImage->draw();
			f.drawDebugString(0, 0, "[READY] : ");

			++mCount;

		}
	}
}