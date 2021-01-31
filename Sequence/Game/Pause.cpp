#include "GameLib/GameLib.h"
#include "GameLib/Framework.h"
using namespace GameLib;

#include "Sequence/Game/Pause.h"
#include "Sequence\Game\GameParent.h"
#include "Sequence\Game\Play.h"

#include "Sequence\Title.h"

#include "Game\KeyBoard.h"

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

		Base* Pause::update(Parent* parent)
		{
			Framework f = Framework::instance();
			Base* next = this;

			if (KeyBoard::isTriggered(KeyBoard::UP))
			{
				--mCursor;
				if (mCursor < 0)
				{
					mCursor = 1;
				}
			}
			else if (KeyBoard::isTriggered(KeyBoard::DOWN))
			{
				++mCursor;
				if (mCursor > 1)
				{
					mCursor = 0;
				}
			}
			else if (KeyBoard::isTriggered(KeyBoard::ACTION))
			{
				if (mCursor == 0)
				{
					next = new Play;
				}
				else if (mCursor == 1)
				{
					next = new Title; // 상위 계층 호출
				}
			}

			parent->drawState();
			mImage->draw();

			f.drawDebugString(0, 0, "MENU");
			f.drawDebugString(1, 2, "PLAY");
			f.drawDebugString(1, 3, "TITLE");
			f.drawDebugString(0, mCursor + 2, ">");

			return next;
		}
	}
}