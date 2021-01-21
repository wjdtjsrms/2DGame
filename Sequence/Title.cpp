#include "GameLib\GameLib.h"
using namespace GameLib;

#include "GameLib\Framework.h"
#include "Sequence\Title.h"
#include "Sequence\Parent.h"
#include "Image.h"
#include "StringRenderer.h"

namespace Sequence
{
	Title::Title()
	{
		mImage = new Image("data/image/dummy.dds");
	}

	Title::~Title()
	{
		SAFE_DELETE(mImage);
	}

	void Title::update(Parent* parent)
	{
		if (Framework::instance().isKeyTriggered(' '))
		{
			parent->moveTo(Parent::SEQ_STAGE_SELECT);
		}
		mImage->draw();

		StringRenderer::Instance()->draw(0, 0, "THIS IS SIMPLE GAME");
		StringRenderer::Instance()->draw(0, 1, "PRESS SPACE BAR");
	}
}