#include "input.h"
#include <string.h>

input_keyboard keyboard;

/************************************************/

void INPUT_ClearKeyboard()
{
}

/************************************************/
void INPUT_Update()
{
	memcpy_s(keyboard.prevkeydown, sizeof(keyboard.prevkeydown), keyboard.keydown, sizeof(keyboard.keydown));
}

/************************************************/

bool INPUT_KeyDown(char key)
{
	return keyboard.keydown[key];
}

bool INPUT_KeyUp(char key)
{
	return !keyboard.keydown[key];
}

bool INPUT_KeyPressed(char key)
{
	return (keyboard.keydown[key] && !keyboard.prevkeydown[key]);
}

bool INPUT_KeyReleased(char key)
{
	return (!keyboard.keydown[key] && keyboard.prevkeydown[key]);
}

/************************************************/