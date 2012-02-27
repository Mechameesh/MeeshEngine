#include "SpriteShader.h"
#include "Shaders.h"
#include "SYS_Math.h"
#include "Graphics.h"
#include <string>

/*********************************************************/

struct shader_spritevertex
{
	math_vec4 pos;
	math_vec2 tex;
};

/*********************************************************/

static gfx_vertexshader * shader_spritevertexshader;
static gfx_pixelshader * shader_spritepixelshader;

static gfx_vertexbuffer * shader_spritevertexbuffer;

static shader_spritevertex shader_spritevertices[6];

/*********************************************************/

void SHADER_InitSpriteShader()
{	
	gfx_vertexdesc desc[2];
	sprintf_s(desc[0].semantic, 32, "POSITION");
	desc[0].vertexformat = GFX_VERTEXFORMAT_RG_FLOAT;
	sprintf_s(desc[1].semantic, 32, "TEXCOORD0");
	desc[1].vertexformat = GFX_VERTEXFORMAT_RG_FLOAT;

	shader_spritevertexshader = GFX_LoadVertexShader("spriteshader.fx", "VS", desc);
	shader_spritepixelshader = GFX_LoadPixelShader("spriteshader.fx", "PS");
	shader_spritevertexbuffer = GFX_CreateVertexBuffer(0, 4, sizeof(float), false);

	for(int i = 0; i < 6; ++i)
		shader_spritevertices[i].pos.z = 0.5;
}

void SHADER_ShutDownSpriteShader()
{
	GFX_FreeVertexBuffer(shader_spritevertexbuffer);	
}

void SHADER_RenderSprite(math_vec2 pos, float width, float height, gfx_texture * texture)
{
	SHADER_RenderSpriteUV(pos, width, height, 0.0f, 0.0f, 1.0f, 1.0f, texture);
}

void SHADER_RenderSpriteUV(math_vec2 pos, float width, float height, float u0, float v0, float u1, float v1, gfx_texture * texture)
{
	GFX_EnableBuffer(GFX_BUFFER_DEPTH, false);

	shader_spritevertices[0].pos.x = pos.x;
	shader_spritevertices[0].pos.y = pos.y;
	shader_spritevertices[1].pos.x = pos.x + width;
	shader_spritevertices[1].pos.y = pos.y;
	shader_spritevertices[2].pos.x = pos.x;
	shader_spritevertices[2].pos.y = pos.y - height;
	shader_spritevertices[3].pos = shader_spritevertices[2].pos;
	shader_spritevertices[4].pos = shader_spritevertices[1].pos;
	shader_spritevertices[5].pos.x = pos.x + width;
	shader_spritevertices[5].pos.y = pos.y - height;

	shader_spritevertices[0].tex.x = u0;
	shader_spritevertices[0].tex.y = v0;
	shader_spritevertices[1].tex.x = u1;
	shader_spritevertices[1].tex.y = v0;
	shader_spritevertices[2].tex.x = u0;
	shader_spritevertices[2].tex.y = v1;
	shader_spritevertices[3].tex = shader_spritevertices[2].tex;
	shader_spritevertices[4].tex = shader_spritevertices[1].tex;
	shader_spritevertices[5].tex.x = u1;
	shader_spritevertices[5].tex.y = v1;

	GFX_SetPrimitiveType(GFX_PRIMITIVETYPE_TRIANGLELIST);
	GFX_DrawPrimitives(shader_spritevertexshader, shader_spritepixelshader, 6);

	GFX_EnableBuffer(GFX_BUFFER_DEPTH, true);
}


