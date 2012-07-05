#ifndef STANDARDSHADER_H_INCLUDED
#define STANDARDSHADER_H_INCLUDED

#include "SYS_Math.h"

struct scene_mesh;
struct gfx_colour;

void SHADER_InitStandardShader();

void SHADER_UpdateStandardShader(const math_matrix4 * wvp, const gfx_colour * ambient);
void SHADER_DrawMeshStandardShader(scene_mesh * mesh);

#endif //STANDARDSHADER_H_INCLUDED