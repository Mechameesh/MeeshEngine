#ifndef INPUT_H_INCLUDED
#define INPUT_H_INCLUDED

#define INPUT_KEYBOARDBUFFER_SIZE 256

#include "SYS_Math.h"

enum
{
	INPUT_MOUSE_LBUTTON,
	INPUT_MOUSE_RBUTTON,
	INPUT_MOUSE_MBUTTON,
	INPUT_MOUSE_NBUTTONS
};

struct input_keyboard
{
	bool keydown[256];
	bool prevkeydown[256];
	char buffer[INPUT_KEYBOARDBUFFER_SIZE];
	int bufferpos;
	bool filter[256];
};

struct input_mouse
{
	bool buttondown[INPUT_MOUSE_NBUTTONS];
	bool prevbuttondown[INPUT_MOUSE_NBUTTONS];
	int xpos, ypos;
	int prevxpos, prevypos;
	int deltax, deltay;
	bool locked;
	int lockx, locky;			//store position when changing between locked and unlocked cursor
};

void INPUT_Init();
void INPUT_Update();

void INPUT_Filter(int whichchar, bool filter);
void INPUT_FilterRange(int startchar, int endchar, bool filter);
void INPUT_ApplyDefaultFilter();
void INPUT_ClearKeyboardBuffer();

bool INPUT_KeyDown(char key);
bool INPUT_KeyUp(char key);
bool INPUT_KeyPressed(char key);
bool INPUT_KeyReleased(char key);

void INPUT_SetMouseLocked(bool locked);

bool INPUT_MouseDown(int mbutton);
bool INPUT_MouseUp(int mbutton);
bool INPUT_MousePressed(int mbutton);
bool INPUT_MouseReleased(int mbutton);

math_vec2 INPUT_GetMousePos();
void INPUT_GetMousePosAbs(int * x, int * y);

math_vec2 INPUT_GetMouseDelta();
void INPUT_GetMouseDeltaAbs(int * x, int * y);

#endif //INPUT_H_INCLUDED