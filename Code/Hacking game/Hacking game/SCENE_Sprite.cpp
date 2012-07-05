#include <stdlib.h>
#include "Shaders.h"
#include "SCENE_Sprite.h"
#include "SCENE_scene.h"
#include "Graphics.h"
#include "TextureCache.h"

static int scene_SpriteCompare(const void *p0, const void *p1)
{
	const scene_sprite *s0, *s1;
	s0 = (scene_sprite*)p0;
	s1 = (scene_sprite*)p1;
	if(s0->priority > s1->priority)
		return 1;
	if(s0->priority < s1->priority)
		return -1;
	return 0;	
}

void SCENE_SortSprites(scene_scene * scene)
{
	qsort(&scene->sprites, scene->nsprites, sizeof(scene_sprite), scene_SpriteCompare);
}

void SCENE_DrawSprites(scene_scene * scene)
{
	float left, right, top, bottom;
	gfx_texture * tex;
	for(int i = 0; i < scene->nsprites; ++i)
	{		
		left = scene->sprites[i].pos.x - (scene->sprites[i].size.x / 2.0f);
		top = scene->sprites[i].pos.y - (scene->sprites[i].size.y / 2.0f);
		right = scene->sprites[i].pos.x + (scene->sprites[i].size.x / 2.0f);
		bottom = scene->sprites[i].pos.y + (scene->sprites[i].size.y / 2.0f);
		
		//do culling here
		if(left > 1.0f || right < 0.0f || top > 1.0f || bottom < 0.0f)
			continue;

		tex = GFX_GetTextureByIndex(scene->spritetextures, scene->sprites[i].texture);
		SHADER_RenderSprite(left, top, scene->sprites[i].size.x,scene->sprites[i].size.y, tex, GFX_GetColour(GFX_COLOUR_WHITE));
	}
}