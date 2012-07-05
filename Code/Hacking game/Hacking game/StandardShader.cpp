#include "StandardShader.h"
#include "SYS_Math.h"
#include "Graphics.h"
#include <string>

struct shader_standardvertex
{
	math_vec3 pos;
	math_vec3 normal;
	math_vec2 tex;
};

struct shader_standardvscbuffer
{
	math_matrix4 wvp;
	gfx_colour ambient;
};

static gfx_vertexshader * shader_standardvertexshader;
static gfx_pixelshader * shader_standardpixelshader;
static gfx_constantbuffer * shader_standardvertexcbuffer;
static shader_standardvscbuffer shader_cbuffer;
static gfx_samplerstate * shader_standardsampler;

void SHADER_InitStandardShader()
{
	gfx_vertexdesc desc[2] = 
	{
		{"POSITION", GFX_VERTEXFORMAT_RGB_FLOAT},		
		{"TEXCOORD", GFX_VERTEXFORMAT_RG_FLOAT}
	};

	shader_standardvertexshader = GFX_LoadVertexShader("data\\shaders\\Standard.fx", "VS", desc, 2);
	shader_standardpixelshader = GFX_LoadPixelShader("data\\shaders\\Standard.fx", "PS");
	shader_standardvertexcbuffer = GFX_CreateConstantBuffer(sizeof(shader_standardvscbuffer));
	
	shader_standardsampler = GFX_CreateSamplerState(GFX_FILTER_MIN_MAG_MIP_LINEAR, GFX_TEXTURE_ADDRESS_WRAP, GFX_TEXTURE_ADDRESS_WRAP, GFX_TEXTURE_ADDRESS_WRAP);

	gfx_colour defaultambient = {0.5f, 0.5f, 0.5f, 1.0f};
	shader_cbuffer.ambient = defaultambient;
}

void SHADER_UpdateStandardShader(const math_matrix4 * wvp, const gfx_colour * ambient)
{
	MATH_Matrix4Copy(&shader_cbuffer.wvp, wvp);
	if(ambient)
		shader_cbuffer.ambient = *ambient;

	GFX_UpdateConstantBuffer(shader_standardvertexcbuffer, &shader_cbuffer);
}

void SHADER_DrawMeshStandardShader(scene_mesh * mesh)
{	
	GFX_SetVertexConstantBuffer(shader_standardvertexcbuffer);
	GFX_SetPixelConstantBuffer(shader_standardvertexcbuffer);
	GFX_SetPrimitiveType(GFX_PRIMITIVETYPE_TRIANGLELIST);
	GFX_SetVertexShader(shader_standardvertexshader);
	GFX_SetPixelShader(shader_standardpixelshader);	

	for(unsigned int i = 0; i < mesh->nsubmeshes; ++i)
	{
		GFX_SetCurrentVertexBuffer(mesh->submeshes[i].vbuffer, 0);
		GFX_SetCurrentIndexBuffer(mesh->submeshes[i].ibuffer, 0);
		for(unsigned int j = 0; j < mesh->submeshes[i].ntextures; ++j)
		{
			GFX_SetTexture(mesh->submeshes[i].textures[j], j);
			GFX_SetSamplerState(j, shader_standardsampler);
		}		
		GFX_DrawPrimitivesIndexed(mesh->submeshes[i].nindices);		
	}
}