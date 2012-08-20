#pragma once

#include "Graphics.h"
#include "SYS_Math.h"

enum 
{
	GUI_INTERACT_UP,
	GUI_INTERACT_DOWN,
	GUI_INTERACT_SELECTED,
};

struct gui_element
{
	math_vec2 pos;
};

struct gui_text : gui_element
{
	char text[256];
};

struct gui_button : gui_element
{
	int state;
	char text[256];
};

struct gui_context
{
};