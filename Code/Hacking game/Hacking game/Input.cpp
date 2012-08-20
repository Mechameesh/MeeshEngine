#include "input.h"
#include "Graphics.h"
#include <string.h>





input_keyboard keyboard;
input_mouse mouse;

/************************************************/

void INPUT_Init()
{
	INPUT_ClearKeyboardBuffer();
	memset(keyboard.keydown, 0, sizeof(keyboard.keydown));
	memset(keyboard.prevkeydown, 0, sizeof(keyboard.prevkeydown));
	for(int i = 0; i < 256; ++i)
	{
		keyboard.filter[i] = true;
	}

	memset(mouse.buttondown, 0, sizeof(mouse.buttondown));
	memset(mouse.prevbuttondown, 0, sizeof(mouse.prevbuttondown));
	mouse.deltax = 0;
	mouse.deltay = 0;
	mouse.locked = false;
	
}

void INPUT_Update()
{
	memcpy_s(keyboard.prevkeydown, sizeof(keyboard.prevkeydown), keyboard.keydown, sizeof(keyboard.keydown));
	memcpy_s(mouse.prevbuttondown, sizeof(mouse.prevbuttondown), mouse.buttondown, sizeof(mouse.buttondown));

	if(!mouse.locked)
	{
		mouse.deltax = mouse.xpos - mouse.prevxpos;
		mouse.deltay = mouse.ypos - mouse.prevypos;
	}

	mouse.prevxpos = mouse.xpos;
	mouse.prevypos = mouse.ypos;
}

/************************************************/

void INPUT_Filter(int whichchar, bool filter)
{
	keyboard.filter[whichchar] = filter;
}

/************************************************/

void INPUT_FilterRange(int startchar, int endchar, bool filter)
{
	if(startchar < 0)
		startchar = 0;
	if(endchar > 255)
		endchar = 255;

	for(int i = startchar; i <= endchar; ++i)
	{
		keyboard.filter[i] = filter;
	}
}

void INPUT_ApplyDefaultFilter()
{
	INPUT_FilterRange(0, 31, false);
	INPUT_FilterRange(32, 125, true);
	INPUT_FilterRange(126, 255, false);
}

void INPUT_ClearKeyboardBuffer()
{
	memset(keyboard.buffer, 0, sizeof(keyboard.buffer));
	keyboard.bufferpos = 0;
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

void INPUT_SetMouseLocked(bool locked)
{	
	if(locked && !mouse.locked)
	{
		mouse.lockx = mouse.xpos;
		mouse.locky = mouse.ypos;
	}
	else if(!locked && mouse.locked)	
	{
		SetCursorPos(mouse.lockx, mouse.locky);
	}

	mouse.locked = locked;
	ShowCursor(!mouse.locked);
}

/************************************************/

bool INPUT_MouseDown(int mbutton)
{
	return mouse.buttondown[mbutton];
}

bool INPUT_MouseUp(int mbutton)
{
	return !mouse.buttondown[mbutton];
}

bool INPUT_MousePressed(int mbutton)
{
	return (mouse.buttondown[mbutton] && !mouse.prevbuttondown[mbutton]);
}

bool INPUT_MouseReleased(int mbutton)
{	
	return (!mouse.buttondown[mbutton] && mouse.prevbuttondown[mbutton]);
}

math_vec2 INPUT_GetMousePos()
{
	float xres = (float)GFX_GetXRes();
	float yres = (float)GFX_GetYRes();

	math_vec2 result;

	result.x = (float)mouse.xpos / xres;
	result.y = (float)mouse.ypos / yres;
	return result;
}

void INPUT_GetMousePosAbs(int * x, int * y)
{
	*x = mouse.xpos;
	*y = mouse.ypos;
}

math_vec2 INPUT_GetMouseDelta()
{
	float xres = (float)GFX_GetXRes();
	float yres = (float)GFX_GetYRes();

	math_vec2 result;

	result.x = (float)mouse.deltax / xres;
	result.y = (float)mouse.deltay / yres;
	return result;
}

void INPUT_GetMouseDeltaAbs(int * x, int * y)
{
	*x = mouse.deltax;
	*y = mouse.deltay;
}
/***********************************************/

