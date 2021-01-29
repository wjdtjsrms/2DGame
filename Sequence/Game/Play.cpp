#include "GameLib/GameLib.h"
#include "GameLib/Input/Manager.h"
#include "GameLib/Input/Keyboard.h"
using namespace GameLib;

#include "Sequence/Game/Play.h"
#include "Sequence\Game\GameParent.h"
#include "Game\State.h"
#include "Game\KeyBoard.h"

namespace Sequence {
	namespace Game {
		Play::Play() { 
		}
		Play::~Play() { 
		}

		void Play::update(Parent* parent) {

			Input::Keyboard kb = Input::Manager::instance().keyboard();
			State* state = parent->state();

			bool cleared = state->hasCleared();
			bool die1P = !state->isAlive(0);
			bool die2P = !state->isAlive(1);

			// 디버그 기능
			if (kb.isTriggered('1')) { // 2P를 죽임
				die2P = true;
			}
			else if (kb.isTriggered('2')) { // 1P를 죽임
				die1P = true;
			}
			else if (kb.isTriggered('c')) {
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
					if (die1P && die2P)
					{
						parent->setWinner(Parent::PLAYER_NONE);
					}
					else if (die1P)
					{
						parent->setWinner(Parent::PLAYER_2);
					}
					else
					{
						parent->setWinner(Parent::PLAYER_1);
					}
				}
			}

			// x를 누르면 메뉴가 나온다.
			if (KeyBoard::isTriggered(KeyBoard::CANCLE))
			{
				parent->moveTo(Parent::NEXT_PAUSE);
			}
			
			state->update();
			parent->drawState();
		}

	}
}
