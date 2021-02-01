#include "GameLib/GameLib.h"
#include "GameLib/Framework.h"
using namespace GameLib;

#include "Game\KeyBoard.h"
#include "Sequence/Title.h"
#include "Sequence/Parent.h"
#include "Image.h"
#include "SoundManager.h"

#include "Sequence\Game\GameParent.h"

namespace Sequence
{
	Title::Title()
		:mImage(0),mCursor(0)
	{
		mImage = new Image("data/image/dummy.dds");
	}
	Title::~Title()
	{
		SAFE_DELETE(mImage);
	}

	Base* Title::update(Parent* parent)
	{
		Base* next = this;
		
		SoundManager* sm = SoundManager::Instance();
		Framework f = Framework::instance();

		if (KeyBoard::isTriggered(KeyBoard::UP))
		{
			--mCursor;
			if (mCursor < 0)
			{
				mCursor = 1;
			}
			sm->playSe(SoundManager::SE_CURSOR_MOVE);
		}
		else if (KeyBoard::isTriggered(KeyBoard::DOWN))
		{
			++mCursor;
			if (mCursor > 1)
			{
				mCursor = 0;
			}
			sm->playSe(SoundManager::SE_CURSOR_MOVE);
		}
		else if (KeyBoard::isTriggered(KeyBoard::ACTION))
		{
			if (mCursor == 0) {
				parent->setMode(Parent::MODE_1P);
			}
			else if (mCursor == 1)
			{
				parent->setMode(Parent::MODE_2P);
			}
			else
			{
				HALT("TITLE ERROR");
			}

			next = new Game::Parent(parent->mode());
			sm->playSe(SoundManager::SE_SELECTION);
		}

		mImage->draw();
		f.drawDebugString(0, 0, "SIMPLE 2D GAME!!");
		f.drawDebugString(1, 2, "1P");
		f.drawDebugString(1, 3, "2P");
		f.drawDebugString(0, mCursor+2, ">");	

		return next;
	}
}