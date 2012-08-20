#include "FlatShader.h"
#include "SYS_Math.h"
#include "Graphics.h"
#include "SCENE_Mesh.h"
#include <string>

struct shader_flatvertex
{
	math_vec3 pos;	
};

struct shader_flatvscbuffer
{
	math_matrix4 wvp;
};

static gfx_vertexshader * shader_flatvertexshader;
static gfx_pixelshader * shader_flatpixelshader;
static gfx_constantbuffer * shader_flatvertexcbuffer;
static shader_flatvscbuffer shader_cbuffer;

void SHADER_InitFlatShader()
{	
	gfx_vertexdesc desc = 
	{
		"POSITION",	GFX_VERTEXFORMAT_RGB_FLOAT
	};

	shader_flatvertexshader = GFX_LoadVertexShader("data\\shaders\\Flat.fx", "VS", &desc, 1);
	shader_flatpixelshader = GFX_LoadPixelShader("data\\shaders\\Flat.fx", "PS");
	shader_flatvertexcbuffer = GFX_CreateConstantBuffer(sizeof(shader_flatvscbuffer));
}

void SHADER_UpdateFlatShader(const math_matrix4 * wvp)
{
	MATH_Matrix4Copy(&shader_cbuffer.wvp, wvp);
	GFX_UpdateConstantBuffer(shader_flatvertexcbuffer, &shader_cbuffer);
		
}

void SHADER_DrawMeshFlatShader(scene_mesh * mesh)
{	
	GFX_SetVertexConstantBuffer(shader_flatvertexcbuffer);
	GFX_SetPrimitiveType(GFX_PRIMITIVETYPE_TRIANGLELIST);
	GFX_SetVertexShader(shader_flatvertexshader);
	GFX_SetPixelShader(shader_flatpixelshader);	

	for(unsigned int i = 0; i < mesh->nsubmeshes; ++i)
	{
		GFX_SetCurrentVertexBuffer(mesh->submeshes[i].vbuffer, 0);
		GFX_SetCurrentIndexBuffer(mesh->submeshes[i].ibuffer, 0);
		GFX_DrawPrimitivesIndexed(mesh->submeshes[i].nindices);
	}
}