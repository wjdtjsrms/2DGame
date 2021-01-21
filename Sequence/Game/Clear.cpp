#include "GameLib\GameLib.h"
using namespace GameLib;

#include "GameLib\Framework.h"
#include "Sequence\Game\Clear.h"
#include "Sequence\Game\GameParent.h"
#include "Image.h"
#include "State.h"
#include "StringRenderer.h"

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

		void Clear::update(Parent* parent)
		{
			if (mCount == 60) // 1ÃÊ ´ë±â
			{
				parent->moveTo(Parent::SEQ_STAGE_SELECT);
			}

			parent->state()->draw();
			mImage->draw();
			Framework::instance().drawDebugString(0, 0, "CLEAR!!");

			++mCount;

		}

	}
}