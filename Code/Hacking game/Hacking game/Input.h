#ifndef INPUT_H_INCLUDED
#define INPUT_H_INCLUDED

struct input_keyboard
{
	bool keydown[256];
	bool prevkeydown[256];
};


void INPUT_ClearKeyboard();
void INPUT_Update();
bool INPUT_KeyDown(char key);
bool INPUT_KeyUp(char key);
bool INPUT_KeyPressed(char key);
bool INPUT_KeyReleased(char key);

#endif //INPUT_H_INCLUDED