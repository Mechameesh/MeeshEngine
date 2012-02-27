#include "SCENE_Sprite.h"
#include "SCENE_scene.h"

static void scene_SortSprites(scene_sprite * sprites, int left, int right)
{
	//quicksort
	int i = left, j = right;
	scene_sprite tmp;

	int pivot = sprites[(left + right) / 2].priority;

	while(i <= j)
	{
		while(sprites[i].priority < pivot)
			i++;
		while(sprites[i].priority > pivot)
			j--;
		if(i <= j)
		{
			tmp = sprites[i];
			sprites[i] = sprites[j];
			sprites[j] = tmp;
			i++;
			j--;
		}
	}

	if(left < j)
		scene_SortSprites(sprites, left, j);
	if(i < right)
		scene_SortSprites(sprites, i, right);
}

void SCENE_SortSprites(scene_scene * scene)
{
	scene_SortSprites(scene->sprites, 0, scene->nsprites - 1);
}