#include "GameLib/GameLib.h"
#include "KeyBoard.h"
#include "GameLib/Input/Manager.h"
#include "GameLib/Input/Keyboard.h"

using namespace GameLib;

bool KeyBoard::isOn(Button b, int pid )
{
	Input::Manager im = Input::Manager::instance();
	Input::Keyboard kb = im.keyboard();
	bool ret = false;

	if (pid == 0) {
		int table[] = { 'd', 'x', 'w', 'z', 'a', 's', 'q' };
		ret = kb.isOn(table[b]);
	}
	else {
		int table[] = { 'l', ',', 'i', 'm', 'j', 'k', 'q' };
		ret = kb.isOn(table[b]);
	}


	return ret;
}

bool KeyBoard::isTriggered(Button b, int pid)
{
	Input::Manager im = Input::Manager::instance();
	Input::Keyboard kb = im.keyboard();
	bool ret = false;

	if (pid == 0) {
		int table[] = { 'd', 'x', 'w', 'z', 'a', 's', 'q' };
		ret = kb.isTriggered(table[b]);
	}
	else {
		int table[] = { 'l', ',', 'i', 'm', 'j', 'k', 'q' };
		ret = kb.isTriggered(table[b]);
	}

	return ret;
}

