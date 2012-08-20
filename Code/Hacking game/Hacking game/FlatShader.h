#ifndef FLATSHADER_H_INCLUDED
#define FLATSHADER_H_INCLUDED

#include "SYS_Math.h"

struct scene_mesh;

void SHADER_InitFlatShader();
void SHADER_UpdateFlatShader(const math_matrix4 * wvp);
void SHADER_DrawMeshFlatShader(scene_mesh * mesh);

#endif //FLATSHADER_H_INCLUDED