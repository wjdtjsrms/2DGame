#include "GameLib/GameLib.h"
#include "Sequence\Game\GameChild.h"
#include "Sequence\Game\GameParent.h"

namespace Sequence
{
	namespace Game {

		Child::~Child()
		{}

		Base* Child::update(Base* p)
		{
			Parent* parent = dynamic_cast<Parent*>(p);
			ASSERT(parent);
			return update(parent);
		}
	}
}
