#include "Shaders.h"
#include "Graphics.h"
#include "SpriteShader.h"
#include "StandardShader.h"

enum
{
	SHADER_SPRITESHADER,
	SHADER_STANDARDSHADER,
	NSHADERS
};


void SHADER_InitShaders()
{
	SHADER_InitSpriteShader();
	SHADER_InitStandardShader();
}