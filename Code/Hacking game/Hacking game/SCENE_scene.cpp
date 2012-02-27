#include "SCENE_scene.h"
#include "SCENE_Sprite.h"
#include <string>

scene_scene * SCENE_CreateScene()
{
	scene_scene * s = (scene_scene *)malloc(sizeof(scene_scene));



	return s;
}

void SCENE_FreeScene(scene_scene * scene)
{
	free(scene);
}

void SCENE_Update(int framedur)
{

}