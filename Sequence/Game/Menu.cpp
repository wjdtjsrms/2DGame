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
				if (mMenuCursor < 0) { // ��ȯ ����
					mMenuCursor = 3;
				}
			}
			else if (f.isKeyTriggered('z')) {
				++mMenuCursor;
				if (mMenuCursor > 3) { // ��ȯ ����
					mMenuCursor = 0;
				}
			}
			else if (f.isKeyTriggered(' ')) {
				switch (mMenuCursor) {
				case 0: // �ٽ� ����
					parent->state()->reset();
					parent->moveTo(Parent::SEQ_PLAY);
					break;
				case 1: // �������� ����
					parent->moveTo(Parent::SEQ_STAGE_SELECT);
					break;
				case 2: // Ÿ��Ʋ ȭ��
					parent->moveTo(Parent::SEQ_TITLE);
					break;
				case 3: // ��� �÷���
					parent->moveTo(Parent::SEQ_PLAY);
					break;
				default: // �������� ����
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
