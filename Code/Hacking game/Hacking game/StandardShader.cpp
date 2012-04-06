#include "StandardShader.h"
#include "SYS_Math.h"
#include "Graphics.h"
#include <string>

struct shader_standardvertex
{
	math_vec3 pos;
	math_vec2 tex;
};

static gfx_vertexshader * shader_standardvertexshader;
static gfx_pixelshader * shader_standardpixelshader;

static gfx_colour shader_ambientcolour;

void SHADER_InitStandardShader()
{
	gfx_vertexdesc desc[2] = 
	{
		{"POSITION", GFX_VERTEXFORMAT_RGBA_FLOAT},
		{"TEXCOORD", GFX_VERTEXFORMAT_RG_FLOAT}
	};

	shader_standardvertexshader = GFX_LoadVertexShader("data\\shaders\\Standard.fx", "VS", desc, 2);
	shader_standardpixelshader = GFX_LoadPixelShader("data\\shaders\\Standard.fx", "PS");
}