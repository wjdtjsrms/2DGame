#include "GameLib/GameLib.h"
#include "GameLib/Input/Manager.h"
#include "GameLib/Input/Keyboard.h"
using namespace GameLib;

#include "Sequence/Game/Play.h"
#include "Sequence\Game\GameParent.h"
#include "Sequence\Game\Clear.h"
#include "Sequence\Game\Failure.h"
#include "Sequence\Game\Judge.h"
#include "Sequence\Game\Pause.h"
#include "SoundManager.h"

#include "Game\State.h"
#include "Game\KeyBoard.h"

namespace Sequence {
	namespace Game {
		Play::Play() { 
			// ���� ������ ��Ÿ ���� ������ ��Ÿ ���� ������ ��Ÿ
			//SoundManager::Instance()->playBgm(SoundManager::BGM_GAME);
		}
		Play::~Play() { 
		}

		Base* Play::update(Parent* parent) {

			Input::Keyboard kb = Input::Manager::instance().keyboard();
			State* state = parent->state();
			Base* next = this;

			bool cleared = state->hasCleared();
			bool die1P = !state->isAlive(0);
			bool die2P = !state->isAlive(1);

			// ����� ���
			if (kb.isTriggered('1')) { // 2P�� ����
				die2P = true;
			}
			else if (kb.isTriggered('2')) { // 1P�� ����
				die1P = true;
			}
			else if (kb.isTriggered('c')) {
				cleared = true;
			}
			
			if (parent->mode() == Parent::MODE_1P) 
			{
				// 1�� ��� �������� Ŭ����
				if (cleared && !die1P) {
					parent->goToNextStage();
					next = new Clear;
				}
				else if (die1P) {
					parent->reduceLife();
					next = new Failure;
				}
			}
			else 
			{   // 2�� ��� �º� ����
				if (die1P || die2P) { 
					next = new Judge;
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

			// x�� ������ �޴��� ���´�.
			if (KeyBoard::isTriggered(KeyBoard::CANCLE))
			{
				next = new Pause;
			}
			
			state->update();
			parent->drawState();

			return next;
		}

	}
}
