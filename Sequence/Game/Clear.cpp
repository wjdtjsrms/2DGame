#include "GameLib\GameLib.h"
#include "GameLib\Framework.h"
using namespace GameLib;

#include "Sequence\Game\Clear.h"
#include "Sequence\Game\GameParent.h"
#include "Sequence\Game\Ready.h"

#include "Image.h"
#include "Game\State.h"

namespace Sequence
{
	namespace Game
	{
		Clear::Clear()
			:mImage(0), mCount(0)
		{
			mImage = new Image("data/image/dummy.dds");
		}

		Clear::~Clear()
		{
			SAFE_DELETE(mImage);
		}

		Child* Clear::update(Parent* parent)
		{
			Child* next = this;
			if (mCount == 60) // 1ÃÊ ´ë±â
			{
				if (parent->hasFinalStageCleared())
				{
					parent->moveTo(Parent::NEXT_ENDING);
				}
				else
				{
					next = new Ready;
				}
			}

			parent->drawState();
			mImage->draw();
			Framework::instance().drawDebugString(0, 0, "CLEAR!!");

			++mCount;
			return next;
		}

	}
}