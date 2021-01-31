#include "GameLib\GameLib.h"
#include "GameLib/Framework.h"
using namespace  GameLib;

#include "Sequence\Game\Failure.h"
#include "Sequence\Game\GameParent.h"
#include "Sequence\Game\Ready.h"

#include "Sequence\GameOver.h"

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

		Base* Failure::update(Parent* parent)
		{
			Base* next = this;

			if (mCount == 60) // 1ÃÊ°¡ Áö³ª¸é
			{
				if (parent->lifeNumber() == 0) // ¸ñ¼ûÀÌ ´Ù ¶³¾îÁ®¼­ »ç¸Á
				{
					next = new GameOver; // »óÀ§ °èÃþ È£Ãâ
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