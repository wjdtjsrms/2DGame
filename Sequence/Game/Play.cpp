#include "GameLib/GameLib.h"
using namespace GameLib;

#include "GameLib/Framework.h"
#include "Sequence/Game/Play.h"
#include "Sequence\Game\GameParent.h"
#include "State.h"

namespace Sequence {
	namespace Game {
		Play::Play() { 
		}
		Play::~Play() { 
		}

		void Play::update(Parent* parent) {
			State* state = parent->state();

			bool cleared = false;
			if (state->hasCleared())
			{
				cleared = true;
			}
			Framework f = Framework::instance();

			// space�� ������ �޴��� ���´�.
			if (f.isKeyTriggered(' '))
			{
				parent->moveTo(Parent::SEQ_MENU);
			}
			else { // ����Ű �Է�
				int dx = 0;
				int dy = 0;
				if (f.isKeyOn('a')) {
					dx -= 1;
				}
				else if (f.isKeyOn('s')) {
					dx += 1;
				}
				else if (f.isKeyOn('w')) {
					dy -= 1;
				}
				else if (f.isKeyOn('z')) {
					dy += 1;
				}
				state->update(dx, dy);

				if (cleared) {
					parent->moveTo(Parent::SEQ_CLEAR);
				}
			}
			// ���� Ȯ���ϱ���
			state->draw();
		}

	}
}
