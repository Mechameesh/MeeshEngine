#ifndef SHADERS_H_INCLUDED
#define SHADERS_H_INCLUDED

#include "SYS_Math.h"
struct gfx_texture;

void SHADER_InitShaders();

void SHADER_RenderSprite(math_vec2 pos, float width, float height, gfx_texture * texture);
void SHADER_RenderSpriteUV(math_vec2 pos, float width, float height, float u0, float v0, float u1, float v1, gfx_texture * texture);



#endif //SHADERS_H_INCLUDED
