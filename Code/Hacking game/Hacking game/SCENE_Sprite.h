#ifndef SCENE_SPRITE_H_INCLUDED
#define SCENE_SPRITE_H_INCLUDED

#include "SYS_Math.h"

/******************************************************/
struct math_vec2;
struct gfx_texture;
/******************************************************/

struct scene_sprite
{
	math_vec2 pos;
	math_vec2 size;
	math_vec2 velocity;
	float rotation;
	float rotatespeed;	
	int texture;
	int priority;
};

/******************************************************/

void SCENE_SortSprites();

#endif //SCENE_SPRITE_H_INCLUDED