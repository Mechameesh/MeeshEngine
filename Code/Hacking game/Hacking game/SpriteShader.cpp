#include "SpriteShader.h"
#include "Shaders.h"
#include "SYS_Math.h"
#include "Graphics.h"
#include <string>

/*********************************************************/

struct shader_spritevertex
{
	math_vec3 pos;
	math_vec2 tex;
	gfx_colour colour;
};

struct shader_spritecbuffer
{
	math_matrix4 wvp;
};

/*********************************************************/

static gfx_vertexshader * shader_spritevertexshader;
static gfx_pixelshader * shader_spritepixelshader;
static gfx_pixelshader * shader_spritepixelcolourshader;
static gfx_constantbuffer * shader_spritevertexcbuffer;

static gfx_samplerstate * shader_spritesampler;
static shader_spritecbuffer shader_cbuffer;

static gfx_vertexbuffer * shader_spritevertexbuffer;

/*********************************************************/

void SHADER_InitSpriteShader()
{		
	gfx_vertexdesc desc[3] = 
	{
		{"POSITION", GFX_VERTEXFORMAT_RGB_FLOAT},
		{"TEXCOORD", GFX_VERTEXFORMAT_RG_FLOAT},
		{"COLOR", GFX_VERTEXFORMAT_RGBA_FLOAT}
	};

	shader_spritevertexshader = GFX_LoadVertexShader("data\\shaders\\spriteshader.fx", "VS", desc, 3);
	shader_spritepixelshader = GFX_LoadPixelShader("data\\shaders\\spriteshader.fx", "PS");	
	shader_spritepixelcolourshader = GFX_LoadPixelShader("data\\shaders\\spriteshader.fx", "PS_Color");
	shader_spritevertexbuffer = GFX_CreateVertexBuffer(0, 6, sizeof(shader_spritevertex), false);
	shader_spritevertexcbuffer = GFX_CreateConstantBuffer(sizeof(shader_spritecbuffer));

	shader_spritesampler = GFX_CreateSamplerState(GFX_FILTER_MIN_MAG_MIP_LINEAR, GFX_TEXTURE_ADDRESS_WRAP, GFX_TEXTURE_ADDRESS_WRAP, GFX_TEXTURE_ADDRESS_WRAP);
}

void SHADER_ShutDownSpriteShader()
{
	GFX_FreeVertexBuffer(shader_spritevertexbuffer);
	GFX_FreeConstantBuffer(shader_spritevertexcbuffer);
}

void SHADER_UpdateSpriteShader(const math_matrix4 * wvp)
{
	MATH_Matrix4Copy(&shader_cbuffer.wvp, wvp);
	GFX_UpdateConstantBuffer(shader_spritevertexcbuffer, &shader_cbuffer);
}

void SHADER_RenderSpriteAbs(int x, int y, int width, int height, const gfx_texture * texture, const gfx_colour * colour)
{
	SHADER_RenderSpriteAbsUV(x, y, width, height, 0.0f, 1.0f, 0.0f, 1.0f, texture, colour);
}

void SHADER_RenderSpriteAbsUV(int x, int y, int width, int height, float u0, float u1, float v0, float v1, const gfx_texture * texture, const gfx_colour * colour)
{
	GFX_EnableBuffer(GFX_BUFFER_DEPTH, false);

	float left = (float)((GFX_GetXRes() / 2) * -1) + (float)x;
	float right = left + (float)width;
	float top = (float)(GFX_GetYRes() / 2) - (float)y;
	float bottom = top - (float)height;

	shader_spritevertex * vbufferdata;
	GFX_LockVertexBufferWrite(shader_spritevertexbuffer, (void**)&vbufferdata);
		
	vbufferdata[0].pos.x = left;
	vbufferdata[0].pos.y = top;	
	vbufferdata[0].pos.z = 0.0f;
	vbufferdata[0].tex.x = u0;
	vbufferdata[0].tex.y = v0;
	vbufferdata[0].colour = *colour;

	vbufferdata[1].pos.x = right;
	vbufferdata[1].pos.y = bottom;
	vbufferdata[1].pos.z = 0.0f;
	vbufferdata[1].tex.x = u1;
	vbufferdata[1].tex.y = v1;
	vbufferdata[1].colour = *colour;

	vbufferdata[2].pos.x = left;
	vbufferdata[2].pos.y = bottom;
	vbufferdata[2].pos.z = 0.0f;
	vbufferdata[2].tex.x = u0;
	vbufferdata[2].tex.y = v1;
	vbufferdata[2].colour = *colour;

	vbufferdata[3].pos = vbufferdata[0].pos;
	vbufferdata[3].tex = vbufferdata[0].tex;
	vbufferdata[3].colour = *colour;

	vbufferdata[4].pos.x = right;
	vbufferdata[4].pos.y = top;
	vbufferdata[4].pos.z = 0.0f;
	vbufferdata[4].tex.x = u1;
	vbufferdata[4].tex.y = v0;
	vbufferdata[4].colour = *colour;

	vbufferdata[5].pos = vbufferdata[1].pos;	
	vbufferdata[5].tex = vbufferdata[1].tex;
	vbufferdata[5].colour = *colour;
	GFX_UnlockVertexBuffer(shader_spritevertexbuffer);

	GFX_SetCurrentVertexBuffer(shader_spritevertexbuffer, 0);
	GFX_SetPrimitiveType(GFX_PRIMITIVETYPE_TRIANGLELIST);
	GFX_SetVertexShader(shader_spritevertexshader);	
	GFX_SetVertexConstantBuffer(shader_spritevertexcbuffer);
	GFX_SetSamplerState(0, shader_spritesampler);

	if(!texture)
	{
		GFX_SetPixelShader(shader_spritepixelcolourshader);
	}
	else
	{
		GFX_SetPixelShader(shader_spritepixelshader);
		GFX_SetTexture(texture, 0);
	}
	GFX_DrawPrimitives(6);

	GFX_EnableBuffer(GFX_BUFFER_DEPTH, true);
}

void SHADER_RenderSprite(float x, float y, float width, float height, const gfx_texture * texture, const gfx_colour * colour)
{
	SHADER_RenderSpriteUV(x, y, width, height, 0.0f, 0.0f, 1.0f, 1.0f, texture, colour);
}

void SHADER_RenderSpriteUV(float x, float y, float width, float height, float u0, float v0, float u1, float v1,const gfx_texture * texture, const gfx_colour * colour)
{
	GFX_EnableBuffer(GFX_BUFFER_DEPTH, false);

	float xres = (float)GFX_GetXRes();
	float yres = (float)GFX_GetYRes();

	float left = ((xres / 2.0f) * - 1.0f) + (x * xres);
	float right = left + (width * xres);
	float top = (yres / 2.0f) - (y * yres);
	float bottom = top - (height * yres);

	shader_spritevertex * vbufferdata;
	GFX_LockVertexBufferWrite(shader_spritevertexbuffer, (void**)&vbufferdata);
		
	vbufferdata[0].pos.x = left;
	vbufferdata[0].pos.y = top;	
	vbufferdata[0].pos.z = 0.0f;
	vbufferdata[0].tex.x = u0;
	vbufferdata[0].tex.y = v0;
	vbufferdata[0].colour = *colour;

	vbufferdata[1].pos.x = right;
	vbufferdata[1].pos.y = bottom;
	vbufferdata[1].pos.z = 0.0f;
	vbufferdata[1].tex.x = u1;
	vbufferdata[1].tex.y = v1;
	vbufferdata[1].colour = *colour;

	vbufferdata[2].pos.x = left;
	vbufferdata[2].pos.y = bottom;
	vbufferdata[2].pos.z = 0.0f;
	vbufferdata[2].tex.x = u0;
	vbufferdata[2].tex.y = v1;
	vbufferdata[2].colour = *colour;

	vbufferdata[3].pos = vbufferdata[0].pos;
	vbufferdata[3].tex = vbufferdata[0].tex;
	vbufferdata[3].colour = *colour;

	vbufferdata[4].pos.x = right;
	vbufferdata[4].pos.y = top;
	vbufferdata[4].pos.z = 0.0f;
	vbufferdata[4].tex.x = u1;
	vbufferdata[4].tex.y = v0;
	vbufferdata[4].colour = *colour;

	vbufferdata[5].pos = vbufferdata[1].pos;	
	vbufferdata[5].tex = vbufferdata[1].tex;
	vbufferdata[5].colour = *colour;

	GFX_UnlockVertexBuffer(shader_spritevertexbuffer);

	GFX_SetCurrentVertexBuffer(shader_spritevertexbuffer, 0);
	GFX_SetPrimitiveType(GFX_PRIMITIVETYPE_TRIANGLELIST);
	GFX_SetVertexShader(shader_spritevertexshader);	
	GFX_SetVertexConstantBuffer(shader_spritevertexcbuffer);
	GFX_SetSamplerState(0, shader_spritesampler);
	
	if(!texture)
	{
		GFX_SetPixelShader(shader_spritepixelcolourshader);		
	}
	else
	{
		GFX_SetPixelShader(shader_spritepixelshader);
		GFX_SetTexture(texture, 0);
	}
	GFX_DrawPrimitives(6);

	GFX_EnableBuffer(GFX_BUFFER_DEPTH, true);
}


