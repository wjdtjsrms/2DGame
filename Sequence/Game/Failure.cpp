#include "GameLib\GameLib.h"
#include "GameLib/Framework.h"
using namespace  GameLib;

#include "Image.h"
#include "Game\State.h"
#include "Sequence\Game\Failure.h"
#include "Sequence\Game\GameParent.h"


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

		void Failure::update(Parent* parent)
		{
			if (mCount == 60) // 1�ʰ� ������
			{
				if (parent->lifeNumber() == 0) // ����� �� �������� ���
				{
					parent->moveTo(Parent::NEXT_GAME_OVER);
				}
				else
				{
					parent->moveTo(Parent::NEXT_READY);
				}

			}

			parent->drawState();
			mImage->draw();
			Framework::instance().drawDebugString(0, 0, "OOPS MISTAKE");
			++mCount;
		}
	}
}