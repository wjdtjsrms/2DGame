#include "GameLib\GameLib.h"
#include "GameLib\Framework.h"
using namespace GameLib;

#include "Sequence\Game\Judge.h"
#include "Sequence\Game\GameParent.h"
#include "Sequence\Game\Ready.h"

#include "Sequence\Title.h"

#include "Game\KeyBoard.h"
#include "Game\State.h"

#include "Image.h"
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

		Base* Judge::update(Parent* parent)
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
					next = new Ready;
				}
				else if (mCursor == 1)
				{
					next = new Title; // 상위 ㅣ계층 호출
				}
			}

			parent->drawState();
			mImage->draw();

			Parent::PlayerID winner = parent->winner();
			if (winner == Parent::PLAYER_1)
			{
				f.drawDebugString(0, 0, "1P WIN");
			}
			else if (winner == Parent::PLAYER_2)
			{
				f.drawDebugString(0, 1, "2P WIN");
			}
			else
			{
				f.drawDebugString(0, 1, "Draw");
			}

			f.drawDebugString(1, 3, "RESTART");
			f.drawDebugString(1, 4, "BACK TITLE");

			f.drawDebugString(0, mCursor + 3, ">");

			return next;
		}
	}
}
