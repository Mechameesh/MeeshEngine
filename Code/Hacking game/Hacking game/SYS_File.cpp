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

void SYS_FileReadVec2(FILE * fp, math_vec2 * v)
{
	fread(&v->x, sizeof(float), 1, fp);
	fread(&v->y, sizeof(float), 1, fp);	
}

void SYS_FileReadVec3(FILE * fp, math_vec3 * v)
{
	fread(&v->x, sizeof(float), 1, fp);
	fread(&v->y, sizeof(float), 1, fp);
	fread(&v->z, sizeof(float), 1, fp);
}

void SYS_FileReadMatrix4(FILE * fp, math_matrix4 * m)
{
	fread(&m->m[0][0], sizeof(float), 1, fp);
	fread(&m->m[0][1], sizeof(float), 1, fp);
	fread(&m->m[0][2], sizeof(float), 1, fp);
	fread(&m->m[0][3], sizeof(float), 1, fp);
	fread(&m->m[1][0], sizeof(float), 1, fp);
	fread(&m->m[1][1], sizeof(float), 1, fp);
	fread(&m->m[1][2], sizeof(float), 1, fp);
	fread(&m->m[1][3], sizeof(float), 1, fp);
	fread(&m->m[2][0], sizeof(float), 1, fp);
	fread(&m->m[2][1], sizeof(float), 1, fp);
	fread(&m->m[2][2], sizeof(float), 1, fp);
	fread(&m->m[2][3], sizeof(float), 1, fp);
	fread(&m->m[3][0], sizeof(float), 1, fp);
	fread(&m->m[3][1], sizeof(float), 1, fp);
	fread(&m->m[3][2], sizeof(float), 1, fp);
	fread(&m->m[3][3], sizeof(float), 1, fp);
}

int SYS_FileReadString(FILE * fp, char * string, int maxlength)
{
	int nchars = 0;	
	while(nchars < maxlength)
	{
		SYS_FileReadChar(fp, &string[nchars]);
		if(!string[nchars])
			break;
		nchars++;		
	}
	return nchars + 1;
}