#include "GameLib/GameLib.h"
using namespace GameLib;

#include "GameLib/Framework.h"
#include "Sequence/Game/Menu.h"
#include "Sequence\Game\GameParent.h"
#include "Image.h"
#include "State.h"
#include "StringRenderer.h"

namespace Sequence {
	namespace Game {

		Menu::Menu() : mImage(0), mMenuCursor(0){
			mImage = new Image("data/image/dummy.dds");
		}

		Menu::~Menu() {
			SAFE_DELETE(mImage);
		}

		void Menu::update(Parent* parent) {
			Framework f = Framework::instance();
			if (f.isKeyTriggered('w')) {
				--mMenuCursor;
				if (mMenuCursor < 0) { // 순환 범위
					mMenuCursor = 3;
				}
			}
			else if (f.isKeyTriggered('z')) {
				++mMenuCursor;
				if (mMenuCursor > 3) { // 순환 범위
					mMenuCursor = 0;
				}
			}
			else if (f.isKeyTriggered(' ')) {
				switch (mMenuCursor) {
				case 0: // 다시 시작
					parent->state()->reset();
					parent->moveTo(Parent::SEQ_PLAY);
					break;
				case 1: // 스테이지 선택
					parent->moveTo(Parent::SEQ_STAGE_SELECT);
					break;
				case 2: // 타이틀 화면
					parent->moveTo(Parent::SEQ_TITLE);
					break;
				case 3: // 계속 플레이
					parent->moveTo(Parent::SEQ_PLAY);
					break;
				default: // 나머지는 무시
					break;
				}
			}		
			parent->state()->draw(); 
			mImage->draw();

			f.drawDebugString(0, 0, "[MENU]");
			f.drawDebugString(1, 1, "RETRY");
			f.drawDebugString(1, 2, "GO TO STAGE SELECTION");
			f.drawDebugString(1, 3, "GO TO TITLE");
			f.drawDebugString(1, 4, "CONTINUE");
			f.drawDebugString(0, mMenuCursor + 1, ">");

		}
	}
}
