#ifndef TEXTURECACHE_H_INCLUDED
#define TEXTURECACHE_H_INCLUDED

struct gfx_texture;

struct gfx_texturecachefile
{
	char filename[80];
	gfx_texture * texture;
};

struct gfx_texturecache
{
	int ntextures;
	gfx_texturecachefile * textures;	
};

gfx_texture * GFX_GetTextureByIndex(gfx_texturecache * cache, const int index);
gfx_texture * GFX_GetTextureByName(gfx_texturecache * cache, const char * filename);

#endif //TEXTURECACHE_H_INCLUDED