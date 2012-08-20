#include <stdio.h>
#include "TextureCache.h"
#include "Graphics.h"

/*********************************************************************/

gfx_texturecache * GFX_LoadTextureCacheFromTxt(const char * filename)
{	
	FILE * fp = 0;
	fopen_s(&fp, filename, "rt");

	if(!fp)
		return 0;

	gfx_texturecache * cache = (gfx_texturecache*)malloc(sizeof(gfx_texturecache));

	fscanf_s(fp, "%d", &cache->ntextures);
	cache->textures = (gfx_texturecachefile*)malloc(sizeof(gfx_texturecachefile) * cache->ntextures);

	for(int i = 0; i < cache->ntextures; ++i)
	{
		fscanf_s(fp, "%s", cache->textures[i].filename);
		cache->textures[i].texture = GFX_LoadTexture(cache->textures[i].filename);
	}

	return cache;
}


gfx_texture * GFX_GetTextureByIndex(gfx_texturecache * cache, const int index)
{
	if(!cache)
		return 0;

	if(index < cache->ntextures)
		return cache->textures[index].texture;
	return 0;
}

gfx_texture * GFX_GetTextureByName(gfx_texturecache * cache, const char * filename)
{
	if(!cache)
		return 0;

	//hashed sort faster? methinks so
	for(int i = 0; i < cache->ntextures; ++i)
	{
		if(strcmp(cache->textures[i].filename, filename) == 0)
			return cache->textures[i].texture;
	}
	return 0;
}

/*********************************************************************/