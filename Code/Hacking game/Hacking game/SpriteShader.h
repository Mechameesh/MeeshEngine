#ifndef SPRITESHADER_H_INCLUDED
#define SPRITESHADER_H_INCLUDED

#include "SYS_Math.h"

void SHADER_InitSpriteShader();
void SHADER_UpdateSpriteShader(const math_matrix4 * wvp);
void SHADER_RenderSpriteShader();

#endif //SPRITESHADER_H_INCLUDED