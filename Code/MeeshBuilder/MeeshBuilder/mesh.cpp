#include "stdafx.h"
#include "mesh.h"
#include "file.h"
#include "texture.h"

const material material_default = {0.5f, 1.0f, 0.0f, 0.0f, 0.0f};

static void mesh_WriteSubMesh(FILE * fp, submesh * in)
{
	FILE_WriteMat4(fp, in->world);

	FILE_WriteUInt(fp, in->nvertices);
	for(unsigned int i = 0; i < in->nvertices; ++i)
	{
		FILE_WriteVec3(fp, in->vertices[i].pos);		
		FILE_WriteVec3(fp, in->vertices[i].normal);
		FILE_WriteVec2(fp, in->vertices[i].uv);
		FILE_WriteVec3(fp, in->vertices[i].tangent);
		FILE_WriteVec3(fp, in->vertices[i].bitangent);
	}
	FILE_WriteUInt(fp, in->nindices);
	for(unsigned int i = 0; i < in->nindices; ++i)
		FILE_WriteUInt(fp, in->indices[i]);	
	FILE_WriteUInt(fp, in->ntextures);
	for(unsigned int i = 0; i < in->ntextures; ++i)
		FILE_WriteString(fp, in->textures[i].filename, 256);
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
	unsigned int i;

	if(m)
	{
		if(m->submeshes)
		{
			for(i = 0; i < m->nsubmeshes; ++i)
			{
				free(m->submeshes[i].vertices);
				free(m->submeshes[i].indices);
			}
			
			free(m->submeshes);
			m->submeshes = 0;
		}
		free(m);
		m = 0;
	}
}

mesh * MESH_ImportMeshesToMesh(vector<importmesh*> * imeshes)
{
	unsigned int i, j;
	mesh * m = (mesh*)malloc(sizeof(mesh));
	memset(m, 0, sizeof(mesh));
	
	m->nsubmeshes = imeshes->size();
	m->submeshes = (submesh *)malloc(sizeof(submesh) * m->nsubmeshes);


	for(i = 0; i < m->nsubmeshes; ++i)
	{
		m->submeshes[i].nvertices = (*imeshes)[i]->vertices.size();
		m->submeshes[i].vertices = (vertex*)malloc(sizeof(vertex) * m->submeshes[i].nvertices);
		for(j = 0; j < m->submeshes[i].nvertices; ++j)
			m->submeshes[i].vertices[j] = (*imeshes)[i]->vertices[j];

		m->submeshes[i].nindices = (*imeshes)[i]->indices.size();
		m->submeshes[i].indices = (unsigned int*)malloc(sizeof(unsigned int) * m->submeshes[i].nindices);
		for(j = 0; j < m->submeshes[i].nindices; ++j)
			m->submeshes[i].indices[j] = (*imeshes)[i]->indices[j];	

		m->submeshes[i].ntextures = (*imeshes)[i]->textures.size();
		m->submeshes[i].textures = (texture*)malloc(sizeof(texture) * m->submeshes[i].ntextures);
		for(j = 0; j < m->submeshes[i].ntextures; ++j)
		{
			strcpy_s(m->submeshes[i].textures[j].filename, 256, (*imeshes)[i]->textures[j].filename);
		}
	}	

	return m;
}

void MESH_ConvertMesh(const char * filename, const char * out)
{
	mesh * m;
	//if(FILE_IsFileExtension(filename, ".dae"))
		//m = MESH_LoadCollada(filename);

	if(FILE_IsFileExtension(filename, ".fbx"))
		m = MESH_LoadFBX(filename);

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