#include "stdafx.h"
#include "mesh.h"
#include "file.h"

static void mesh_WriteSubMesh(FILE * fp, submesh * in)
{
	FILE_WriteMat4(fp, in->world);

	FILE_WriteUInt(fp, in->nvertices);
	for(unsigned int i = 0; i < in->nvertices; ++i)
	{
		FILE_WriteVec3(fp, in->vertices[i].pos);
		FILE_WriteVec2(fp, in->vertices[i].uv);
		FILE_WriteVec3(fp, in->vertices[i].normal);
		FILE_WriteVec3(fp, in->vertices[i].tangent);
		FILE_WriteVec3(fp, in->vertices[i].bitangent);
	}
	FILE_WriteUInt(fp, in->nindices);
	for(unsigned int i = 0; i < in->nindices; ++i)
		FILE_WriteUInt(fp, in->indices[i]);	
}

static void mesh_WriteMesh(mesh * in, const char * filename)
{
	FILE * fp = 0;
	fopen_s(&fp, filename, "wb");

	if(!fp)
	{
		printf("Failed to write \"%s\"", filename);
		return;
	}

	FILE_WriteUInt(fp, in->nsubmeshes);
	
	for(unsigned int i = 0; i < in->nsubmeshes; ++i)
	{
		mesh_WriteSubMesh(fp, &in->submeshes[i]);
	}

	
	fclose(fp);
}

void MESH_FreeMesh(mesh * m)
{
	if(m)
	{
		if(m->submeshes)
		{
			free(m->submeshes);
			m->submeshes = 0;
		}
	}
}

void MESH_ConvertMesh(const char * filename, const char * out)
{
	mesh * m;
	if(FILE_IsFileExtension(filename, ".dae"))
		m = MESH_LoadCollada(filename);

	if(!m)
		return;

	if(!out)
	{
		char newname[64];	
		int length = strlen(filename);
		strcpy_s(newname, sizeof(newname), filename);
		newname[length - 3] = 'm';
		newname[length - 2] = 's';
		newname[length - 1] = 'h';
		mesh_WriteMesh(m, newname);
	}
	else
		mesh_WriteMesh(m, out);
}