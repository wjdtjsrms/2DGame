#include "GameLib/GameLib.h"
using namespace GameLib;

#include "GameLib/Framework.h"
#include "Sequence/Game/Play.h"
#include "Sequence\Game\GameParent.h"
#include "Game\State.h"

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

			// 디버그 기능
			if (f.isKeyTriggered('1')) { //1P 승리
				die2P = true;
			}
			else if (f.isKeyTriggered('2') || f.isKeyTriggered('x')) { //2P 승리
				die1P = true;
			}
			else if (f.isKeyTriggered('c')) { // 스테이지 클리어
				cleared = true;
			}
			
			if (parent->mode() == Parent::MODE_1P) 
			{
				// 1인 모드 스테이지 클리어
				if (cleared && !die1P) {
					parent->moveTo(Parent::NEXT_CLEAR);
				}
				else if (die1P) {
					parent->moveTo(Parent::NEXT_FAILURE);
				}
			}
			else 
			{   // 2인 모드 승부 결정
				if (die1P || die2P) { 
					parent->moveTo(Parent::NEXT_JUDGE);
				}
			}

			// space를 누르면 메뉴가 나온다.
			if (f.isKeyTriggered(' '))
			{
				parent->moveTo(Parent::NEXT_PAUSE);
			}
			
			state->update();
			// 성능 확실하구만
			state->draw();
		}

	}
}
