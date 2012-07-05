#include "stdafx.h"
#include "file.h"

void FILE_ReadInt(FILE * fp, int * n)
{
	fread(&n, sizeof(int), 1, fp);
}

void FILE_ReadUInt(FILE * fp, unsigned int * n)
{
	fread(n, sizeof(unsigned int), 1, fp);
}

void FILE_ReadFloat(FILE * fp, float * n)
{
	fread(n, sizeof(float), 1, fp);
}

void FILE_ReadShort(FILE * fp, short * n)
{
	fread(n, sizeof(short), 1, fp);
}

void FILE_ReadUShort(FILE * fp, unsigned short * n)
{
	fread(n, sizeof(unsigned short), 1, fp);
}

void FILE_ReadChar(FILE * fp, char * c)
{
	fread(c, sizeof(char), 1, fp);
}

void FILE_ReadUChar(FILE * fp, unsigned char * c)
{
	fread(c, sizeof(unsigned char), 1, fp);
}

void FILE_ReadVec2(FILE * fp, math_vec2 * v)
{
	FILE_ReadFloat(fp, &v->x);
	FILE_ReadFloat(fp, &v->y);	
}

void FILE_ReadVec3(FILE * fp, math_vec3 * v)
{
	FILE_ReadFloat(fp, &v->x);
	FILE_ReadFloat(fp, &v->y);
	FILE_ReadFloat(fp, &v->z);
}

void FILE_ReadMat4(FILE * fp, math_matrix4 * m)
{
	FILE_ReadFloat(fp, &m->m[0][0]);
	FILE_ReadFloat(fp, &m->m[0][1]);
	FILE_ReadFloat(fp, &m->m[0][2]);
	FILE_ReadFloat(fp, &m->m[0][3]);
	FILE_ReadFloat(fp, &m->m[1][0]);
	FILE_ReadFloat(fp, &m->m[1][1]);
	FILE_ReadFloat(fp, &m->m[1][2]);
	FILE_ReadFloat(fp, &m->m[1][3]);
	FILE_ReadFloat(fp, &m->m[2][0]);
	FILE_ReadFloat(fp, &m->m[2][1]);
	FILE_ReadFloat(fp, &m->m[2][2]);
	FILE_ReadFloat(fp, &m->m[2][3]);
	FILE_ReadFloat(fp, &m->m[3][0]);
	FILE_ReadFloat(fp, &m->m[3][1]);
	FILE_ReadFloat(fp, &m->m[3][2]);
	FILE_ReadFloat(fp, &m->m[3][3]);
}

int FILE_ReadString(FILE * fp, char * string, int maxlength)
{
	int nchars = 0;	
	while(nchars < maxlength)
	{
		FILE_ReadChar(fp, &string[nchars]);
		if(!string[nchars])
			break;
		nchars++;		
	}
	return nchars + 1;
}

/********************************************************/

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

void FILE_WriteChar(FILE * fp, char c)
{
	fwrite(&c, sizeof(char), 1, fp);
}

void FILE_WriteUChar(FILE * fp, unsigned char c)
{
	fwrite(&c, sizeof(unsigned char), 1, fp);
}

void FILE_WriteVec2(FILE * fp, math_vec2 v)
{
	FILE_WriteFloat(fp, v.x);
	FILE_WriteFloat(fp, v.y);
}

void FILE_WriteVec3(FILE * fp, math_vec3 v)
{
	FILE_WriteFloat(fp, v.x);
	FILE_WriteFloat(fp, v.y);
	FILE_WriteFloat(fp, v.z);
}

void FILE_WriteMat4(FILE * fp, math_matrix4 m)
{
	FILE_WriteFloat(fp, m.m[0][0]);
	FILE_WriteFloat(fp, m.m[0][1]);
	FILE_WriteFloat(fp, m.m[0][2]);
	FILE_WriteFloat(fp, m.m[0][3]);
	FILE_WriteFloat(fp, m.m[1][0]);
	FILE_WriteFloat(fp, m.m[1][1]);
	FILE_WriteFloat(fp, m.m[1][2]);
	FILE_WriteFloat(fp, m.m[1][3]);
	FILE_WriteFloat(fp, m.m[2][0]);
	FILE_WriteFloat(fp, m.m[2][1]);
	FILE_WriteFloat(fp, m.m[2][2]);
	FILE_WriteFloat(fp, m.m[2][3]);
	FILE_WriteFloat(fp, m.m[3][0]);
	FILE_WriteFloat(fp, m.m[3][1]);
	FILE_WriteFloat(fp, m.m[3][2]);
	FILE_WriteFloat(fp, m.m[3][3]);
}

int FILE_WriteString(FILE * fp, char * string, int maxlength)
{
	int nchars = 0;	
	while(nchars < maxlength)
	{
		
		FILE_WriteChar(fp, string[nchars]);
		if(!string[nchars])
			break;
		nchars++;
	}
	return nchars + 1;
}

/********************************************************/

size_t FILE_GetFileSize(FILE * fp)
{
	fseek(fp, 0, SEEK_SET);
	fseek(fp, 0, SEEK_END);
	size_t ret = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	return ret;
}

bool FILE_IsFileExtension(const char * filename, const char * extension)
{
	int length = strlen(filename);
	int extlength = strlen(extension);

	return (strcmp(&filename[length - extlength], extension) == 0);	
}