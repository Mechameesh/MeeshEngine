#include "Shaders.h"
#include "Graphics.h"
#include "SpriteShader.h"
#include "StandardShader.h"
#include "FlatShader.h"

void SHADER_InitShaders()
{
	SHADER_InitSpriteShader();
	//SHADER_InitStandardShader();
	//SHADER_InitFlatShader();
}

void SHADER_UpdateShaders(const math_matrix4 * wvp)
{
	SHADER_UpdateSpriteShader(wvp);
	//SHADER_UpdateStandardShader(wvp);
	//SHADER_UpdateFlatShader(wvp);
}

void SHADER_DrawMesh(scene_mesh * mesh, int shader)
{
	switch(shader)
	{
		case SHADER_SPRITESHADER: break;
		case SHADER_STANDARDSHADER: break;
		case SHADER_FLATSHADER: SHADER_DrawMeshFlatShader(mesh);
		default: break;
	}
}