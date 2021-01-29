#ifndef INCLUDED_KEYBOARD_H
#define INCLUDED_KEYBOARD_H

class KeyBoard
{
public:
	enum Button {
		ACTION, //'d'
		CANCLE, //'x'
		UP, 
		DOWN, 
		LEFT, 
		RIGHT, 
		QUIT,
	};

	static bool isOn(Button, int playerID = 0);
	static bool isTriggered(Button, int playerID = 0);

};
#endif