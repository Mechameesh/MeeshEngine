#include "SYS_File.h"
#include <stdio.h>
#include <string.h>
#include "SYS_Math.h"

bool SYS_IsFileExtension(const char * filename, const char * extension)
{
	int length = strlen(filename);
	int extlength = strlen(extension);

	return (strcmp(&filename[length - extlength], extension) == 0);	
}

void SYS_FileReadChar(FILE * fp, char * c)
{
	fread(c, sizeof(char), 1, fp);
}

void SYS_FileReadUChar(FILE * fp, unsigned char * c)
{
	fread(c, sizeof(unsigned char), 1, fp);
}

void SYS_FileReadShort(FILE * fp, short * s)
{
	fread(s, sizeof(short), 1, fp);
}

void SYS_FileReadUShort(FILE * fp, unsigned short * s)
{
	fread(s, sizeof(unsigned short), 1, fp);
}

void SYS_FileReadInt(FILE * fp, int * i)
{
	fread(i, sizeof(int), 1, fp);
}

void SYS_FileReadUInt(FILE * fp, unsigned int * i)
{
	fread(i, sizeof(unsigned int), 1, fp);
}

void SYS_FileReadFloat(FILE * fp, float * f)
{
	fread(f, sizeof(float), 1, fp);
}

void SYS_FileReadVec3(FILE * fp, math_vec3 * v)
{
	fread(&v->x, sizeof(float), 1, fp);
	fread(&v->y, sizeof(float), 1, fp);
	fread(&v->z, sizeof(float), 1, fp);
}