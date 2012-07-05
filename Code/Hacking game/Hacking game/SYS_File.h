#ifndef SYS_FILE_H_INCLUDED
#define SYS_FILE_H_INCLUDED

#include <stdio.h>

struct math_vec2;
struct math_vec3;
struct math_matrix4;

bool SYS_IsFileExtension(const char * filename, const char * extension);

void SYS_FileReadChar(FILE * fp, char * c);
void SYS_FileReadUChar(FILE * fp, unsigned char * c);
void SYS_FileReadShort(FILE * fp, short * s);
void SYS_FileReadUShort(FILE * fp, unsigned short * s);
void SYS_FileReadInt(FILE * fp, int * i);
void SYS_FileReadUInt(FILE * fp, unsigned int * i);
void SYS_FileReadFloat(FILE * fp, float * f);
void SYS_FileReadVec2(FILE * fp, math_vec2 * v);
void SYS_FileReadVec3(FILE * fp, math_vec3 * v);
void SYS_FileReadMatrix4(FILE * fp, math_matrix4 * m);
int SYS_FileReadString(FILE * fp, char * string, int maxlength);

#endif //SYS_FILE_H_INCLUDED