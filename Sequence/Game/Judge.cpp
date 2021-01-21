#include "GameLib\GameLib.h"
#include "GameLib\Framework.h"
using namespace GameLib;

#include "Image.h"
#include "State.h"
#include "Sequence\Game\Judge.h"
#include "Sequence\Game\GameParent.h"


namespace Sequence
{
	namespace Game
	{
		Judge::Judge()
			:mImage(0), mCursor(0)
		{
			mImage = new Image("data/image/dummy.dds");
		}

		Judge::~Judge()
		{
			SAFE_DELETE(mImage);
		}

		void Judge::update(Parent* parent)
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
					parent->moveTo(Parent::NEXT_READY);
				}
				else if (mCursor == 1)
				{
					parent->moveTo(Parent::NEXT_TITLE);
				}
			}

			parent->state()->draw();
			mImage->draw();


			f.drawDebugString(0, 0, "IF IP WIN");
			f.drawDebugString(0, 1, "IF IP WIN");

			f.drawDebugString(1, 3, "RESTART");
			f.drawDebugString(1, 4, "BACK TITLE");

			f.drawDebugString(0, mCursor + 3, ">");


		}
	}
}
