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
			Framework f = Framework::instance();




			bool cleared = state->hasCleared();
			bool die1P = false;
			bool die2P = false;

			// ����� ���
			if (f.isKeyTriggered('1')) { //1P �¸�
				die2P = true;
			}
			else if (f.isKeyTriggered('2') || f.isKeyTriggered('x')) { //2P �¸�
				die1P = true;
			}
			else if (f.isKeyTriggered('c')) { // �������� Ŭ����
				cleared = true;
			}
			
			if (parent->mode() == Parent::MODE_1P) 
			{
				// 1�� ��� �������� Ŭ����
				if (cleared && !die1P) {
					parent->moveTo(Parent::NEXT_CLEAR);
				}
				else if (die1P) {
					parent->moveTo(Parent::NEXT_FAILURE);
				}
			}
			else 
			{   // 2�� ��� �º� ����
				if (die1P || die2P) { 
					parent->moveTo(Parent::NEXT_JUDGE);
				}
			}

			// space�� ������ �޴��� ���´�.
			if (f.isKeyTriggered(' '))
			{
				parent->moveTo(Parent::NEXT_PAUSE);
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
					parent->moveTo(Parent::NEXT_CLEAR);
				}
			}
			// ���� Ȯ���ϱ���
			state->draw();
		}

	}
}
