#ifndef SCENE_SCENE_H_INCLUDED
#define SCENE_SCENE_H_INCLUDED
#include "SCENE_Camera.h"
#include "SCENE_Sprite.h"

/*******************************************************************/

#define MAX_CAMERAS 8
#define MAX_SPRITES 1024

/*******************************************************************/

struct scene_scene
{
	scene_camera camera[MAX_CAMERAS];
	int currentcamera;

	scene_sprite sprites[MAX_SPRITES];
	int nsprites;
};

/*******************************************************************/

scene_scene * SCENE_CreateScene();
void SCENE_Freescene(scene_scene * scene);

void SCENE_Update(int framedur);

#endif //SCENE_SCENE_H_INCLUDED