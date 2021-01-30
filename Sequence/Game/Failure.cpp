#include "GameLib\GameLib.h"
#include "GameLib/Framework.h"
using namespace  GameLib;

#include "Sequence\Game\Failure.h"
#include "Sequence\Game\GameParent.h"
#include "Sequence\Game\Ready.h"

#include "Game\State.h"
#include "Image.h"

namespace Sequence
{
	namespace Game
	{
		Failure::Failure()
			:mImage(0),mCount(0)
		{
			mImage = new Image("data/image/dummy.dds");
		}

		Failure::~Failure()
		{
			SAFE_DELETE(mImage);
		}

		Child* Failure::update(Parent* parent)
		{
			Child* next = this;

			if (mCount == 60) // 1ÃÊ°¡ Áö³ª¸é
			{
				if (parent->lifeNumber() == 0) // ¸ñ¼ûÀÌ ´Ù ¶³¾îÁ®¼­ »ç¸Á
				{
					parent->moveTo(Parent::NEXT_GAME_OVER);
				}
				else
				{
					next = new Ready;
				}

			}
			parent->drawState();
			mImage->draw();
			Framework::instance().drawDebugString(0, 0, "OOPS MISTAKE");
			++mCount;

			return next;
		}
	}
}