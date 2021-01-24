#include "GameLib/GameLib.h"
#include "GameLib/Framework.h"
using namespace GameLib;

#include "Sequence/Game/Pause.h"
#include "Sequence\Game\GameParent.h"
#include "Image.h"
#include "Game\State.h"

namespace Sequence
{
	namespace  Game
	{
		Pause::Pause()
			:mImage(0), mCursor(0)
		{
			mImage = new Image("data/image/dummy.dds");
		}

		Pause::~Pause()
		{
			SAFE_DELETE(mImage);
		}

		void Pause::update(Parent* parent)
		{
			Framework f = Framework::instance();

			if (f.isKeyTriggered('w'))
			{
				--mCursor;
				if (mCursor < 0)
				{
					mCursor = 1;
				}
			}
			else if (f.isKeyTriggered('z'))
			{
				++mCursor;
				if (mCursor > 1)
				{
					mCursor = 0;
				}
			}
			else if (f.isKeyTriggered(' '))
			{
				if (mCursor == 0)
				{
					parent->moveTo(Parent::NEXT_PLAY);
				}
				else if (mCursor == 1)
				{
					parent->moveTo(Parent::NEXT_TITLE);
				}
			}

			parent->state()->draw();
			mImage->draw();

			f.drawDebugString(0, 0, "MENU");
			f.drawDebugString(1, 2, "PLAY");
			f.drawDebugString(1, 3, "TITLE");
			f.drawDebugString(0, mCursor + 2, ">");


		}
	}
}