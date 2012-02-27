#include "stdafx.h"
#include "mesh.h"
#include "file.h"

struct chunk_3ds
{
	unsigned short id;
	unsigned int length;
};

static int GetFileSize(FILE * fp)
{
	fseek(fp, 0, SEEK_SET);
	fseek(fp, 0, SEEK_END);
	size_t ret = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	return ret;
}

static mesh * Load3DS(const char * filename)
{
	FILE * fp = fopen(filename, "rb");
	if(!fp)
	{
		printf("Failed to load \"%s\"", filename);
		return 0;
	}

	int size = GetFileSize(fp);

	chunk_3ds chunk;	

	mesh * m = (mesh *)malloc(sizeof(mesh));
	memset(m, 0, sizeof(mesh));
	m->parent = -1;

	while(ftell(fp) < size)
	{
		fread(&chunk, sizeof(chunk), 1, fp);
		
		switch(chunk.id)
		{
		case 0x4d4d: break;		//MAIN CHUNK
		case 0x3d3d: break;		//3D EDITOR CHUNK
		case 0x4000:			//OBJECT BLOCK
			{	
				char c;
				do
				{
					fread(&c, 1, 1, fp);
				} while(c != '\0');
			} break;
		case 0x4100: break;		//TRIANGULAR MESH
		case 0x4110:			//VERTEX LIST
			{
				unsigned short nverts;
				fread(&nverts, sizeof(unsigned short), 1, fp);				
				m->nvertices = (int)nverts;

				m->vertices = (vec3 *)malloc(sizeof(vec3) * nverts);

				for(int i = 0; i < m->nvertices; ++i)
				{
					fread(&m->vertices[i].x, sizeof(float), 1, fp);
					fread(&m->vertices[i].y, sizeof(float), 1, fp);
					fread(&m->vertices[i].z, sizeof(float), 1, fp);
				}
			} break;
		case 0x4120:			//FACES DESCRIPTION
			{
				unsigned short ntris, index;
				fread(&ntris, sizeof(unsigned short), 1, fp);				
				m->ntris = (int)ntris;
				m->nindices =  m->ntris * 3;
					

				m->indices = (int *)malloc(sizeof(int) * m->nindices * 3);

				for(int i = 0; i < m->nindices; i += 3)
				{
					fread(&index, sizeof(unsigned short), 1, fp);
					m->indices[i] = (int)index;
					fread(&index, sizeof(unsigned short), 1, fp);
					m->indices[i + 1] = (int)index;
					fread(&index, sizeof(unsigned short), 1, fp);
					m->indices[i + 2] = (int)index;
				}
			} break;
		case 0x4140:			//MAPPING COORDINATES LIST
			{
				unsigned short nuvs;
				fread(&nuvs, sizeof(unsigned short), 1, fp);
				for(int i = 0; i < nuvs; ++i)
				{
					fread(&m->uv[i], sizeof(uv), 1, fp);
				}
			} break;
		default:
			fseek(fp, chunk.length - 6, SEEK_CUR);
		}		
	}	

	fclose(fp);
	return m;
}

static void WriteMesh(mesh * in, const char * filename)
{
	FILE * fp = fopen(filename, "wb");

	if(!fp)
	{
		printf("Failed to write \"%s\"", filename);
		return;
	}

	FILE_WriteInt(fp, in->nvertices);
	for(int i = 0; i < in->nvertices; ++i)
		FILE_WriteVec3(fp, in->vertices[i]);

	FILE_WriteInt(fp, in->nindices);
	for(int i = 0; in->nindices; ++i)
		FILE_WriteInt(fp, in->indices[i]);	
	
	fclose(fp);
}

static void FreeMesh(mesh * m)
{
	if(m)
	{
		if(m->vertices)
		{
			free(m->vertices);
			m->vertices = 0;
		}
		if(m->indices)
		{
			free(m->indices);
			m->indices = 0;
		}
		if(m->uv)
		{
			free(m->uv);
			m->uv = 0;
		}
	}
}

void Convert3DS(const char * filename)
{
	mesh * 	m = Load3DS(filename);
	if(!m)	
		return;
	char newname[64];	
	int length = strlen(filename);
	strcpy(newname, filename);
	newname[length - 3] = 'm';
	newname[length - 2] = 's';
	newname[length - 1] = 'h';
	WriteMesh(m, newname);
}