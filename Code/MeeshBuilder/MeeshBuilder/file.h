#pragma once
#include "mesh.h"

void FILE_ReadInt(FILE * fp, int * n);
void FILE_ReadUInt(FILE * fp, unsigned int * n);
void FILE_ReadFloat(FILE * fp, float * n);
void FILE_ReadShort(FILE * fp, short * n);
void FILE_ReadUShort(FILE * fp, unsigned short * n);
void FILE_ReadChar(FILE * fp, char * c);
void FILE_ReadUChar(FILE * fp, unsigned char * c);
void FILE_ReadVec2(FILE * fp, math_vec2 * v);
void FILE_ReadVec3(FILE * fp, math_vec3 * v);
void FILE_ReadMat4(FILE * fp, math_matrix4 * m);

void FILE_WriteInt(FILE * fp, int n);
void FILE_WriteUInt(FILE * fp, unsigned int n);
void FILE_WriteFloat(FILE * fp, float n);
void FILE_WriteShort(FILE * fp, short n);
void FILE_WriteUShort(FILE * fp, unsigned short n);
void FILE_WriteChar(FILE * fp, char c);
void FILE_WriteUChar(FILE * fp, unsigned char c);
void FILE_WriteVec2(FILE * fp, math_vec2 v);
void FILE_WriteVec3(FILE * fp, math_vec3 v);
void FILE_WriteMat4(FILE * fp, math_matrix4 m);

size_t FILE_GetFileSize(FILE * fp);
bool FILE_IsFileExtension(const char * filename, const char * extension);

mesh * Load3DS(const char * filename);