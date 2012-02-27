#pragma once
#include "mesh.h"

void FILE_WriteInt(FILE * fp, int n);
void FILE_WriteUInt(FILE * fp, unsigned int n);
void FILE_WriteFloat(FILE * fp, float n);
void FILE_WriteShort(FILE * fp, short n);
void FILE_WriteUShort(FILE * fp, unsigned short n);
void FILE_WriteUShort(FILE * fp, char c);
void FILE_WriteUChar(FILE * fp, unsigned char c);
void FILE_WriteVec3(FILE * fp, vec3 v);