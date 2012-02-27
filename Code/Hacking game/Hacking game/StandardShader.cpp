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
	gfx_vertexdesc desc[2];
	sprintf_s(desc[0].semantic, 32, "POSITION");
	desc[0].vertexformat = GFX_VERTEXFORMAT_RGBA_FLOAT;
	sprintf_s(desc[1].semantic, 32, "TEXCOORD0");
	desc[1].vertexformat = GFX_VERTEXFORMAT_RG_FLOAT;

	shader_standardvertexshader = GFX_LoadVertexShader("Standard.fx", "VS", desc);
	shader_standardpixelshader = GFX_LoadPixelShader("Standard.fx", "PS");
}