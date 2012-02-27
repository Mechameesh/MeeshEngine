#include "stdafx.h"
#include "file.h"

void FILE_WriteInt(FILE * fp, int n)
{
	fwrite(&n, sizeof(int), 1, fp);
}

void FILE_WriteUInt(FILE * fp, unsigned int n)
{
	fwrite(&n, sizeof(unsigned int), 1, fp);
}

void FILE_WriteFloat(FILE * fp, float n)
{
	fwrite(&n, sizeof(float), 1, fp);
}

void FILE_WriteShort(FILE * fp, short n)
{
	fwrite(&n, sizeof(short), 1, fp);
}

void FILE_WriteUShort(FILE * fp, unsigned short n)
{
	fwrite(&n, sizeof(unsigned short), 1, fp);
}

void FILE_WriteUShort(FILE * fp, char c)
{
	fwrite(&c, sizeof(char), 1, fp);
}

void FILE_WriteUChar(FILE * fp, unsigned char c)
{
	fwrite(&c, sizeof(unsigned char), 1, fp);
}

void FILE_WriteVec3(FILE * fp, vec3 v)
{
	FILE_WriteFloat(fp, v.x);
	FILE_WriteFloat(fp, v.y);
	FILE_WriteFloat(fp, v.z);
}