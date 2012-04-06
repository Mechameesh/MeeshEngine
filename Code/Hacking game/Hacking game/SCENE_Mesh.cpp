#include "SCENE_Mesh.h"
#include "SYS_File.h"
#include "Graphics.h"
#include <string>

static scene_mesh * scene_LoadMeshMsh(const char * filename)
{
	FILE * fp = fopen(filename, "rb");
	if(!fp)
		return 0;

	scene_mesh * m = (scene_mesh *)malloc(sizeof(scene_mesh));
	memset(m, 0, sizeof(scene_mesh));	
	m->parent = -1;
		
	SYS_FileReadUInt(fp, &m->nvertices);
	m->vertices = (math_vec3 *)malloc(sizeof(math_vec3) * m->nvertices);
	for(unsigned int i = 0; i < m->nvertices; ++i)
	{
		SYS_FileReadVec3(fp, &m->vertices[i]);
	}

	SYS_FileReadUInt(fp, &m->nindices);
	m->indices = (unsigned int *)malloc(sizeof(unsigned int) * m->nindices);
	for(unsigned int i = 0; i < m->nindices; ++i)
	{
		SYS_FileReadUInt(fp, &m->indices[i]);
	}

	fclose(fp);

	m->ntris = m->nindices / 3;

	m->vbuffer = GFX_CreateVertexBuffer(m->vertices, m->nvertices, sizeof(math_vec3), true);	
	m->ibuffer = GFX_CreateIndexBuffer(m->indices, m->nindices, true);

	return m;
}

/******************************************************************/

scene_mesh * SCENE_LoadMesh(const char * filename)
{
	if(SYS_IsFileExtension(filename, ".msh"))
		return scene_LoadMeshMsh(filename);		
	
	return 0;	
}

void SCENE_FreeMesh(scene_mesh * mesh)
{
	if(mesh)
	{
		if(mesh->vertices)
			free(mesh->vertices);
		if(mesh->normals)
			free(mesh->normals);
		if(mesh->uv)
			free(mesh->uv);
		if(mesh->indices)
			free(mesh->indices);

		free(mesh);

		mesh = 0;
	}
}

/******************************************************************/