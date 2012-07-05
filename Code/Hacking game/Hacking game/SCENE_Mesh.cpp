#include "SCENE_Mesh.h"
#include "SYS_File.h"
#include "Graphics.h"
#include "Debug.h"
#include <string>

#define FOLDER_MODELS "data/models/"

static scene_mesh * scene_LoadMeshMsh(const char * filename)
{
	FILE * fp = 0;
	fopen_s(&fp, filename, "rb");
	if(!fp)
		return 0;
	
	scene_mesh * m = (scene_mesh *)malloc(sizeof(scene_mesh));
	memset(m, 0, sizeof(scene_mesh));		
	
	SYS_FileReadUInt(fp, &m->nsubmeshes);
	m->submeshes = (scene_submesh *)malloc(sizeof(scene_submesh) * m->nsubmeshes);
	memset(m->submeshes, 0, sizeof(scene_submesh) * m->nsubmeshes);

	for(unsigned int i = 0; i < m->nsubmeshes; ++i)
	{
		SYS_FileReadMatrix4(fp, &m->submeshes[i].world);
		SYS_FileReadUInt(fp, &m->submeshes[i].nvertices);
		m->submeshes[i].vertices = (scene_vertex*)malloc(sizeof(scene_vertex) * m->submeshes[i].nvertices);


		for(unsigned int j = 0; j < m->submeshes[i].nvertices; ++j)
		{
			SYS_FileReadVec3(fp, &m->submeshes[i].vertices[j].pos);			
			SYS_FileReadVec3(fp, &m->submeshes[i].vertices[j].normal);
			SYS_FileReadVec2(fp, &m->submeshes[i].vertices[j].uv);
			SYS_FileReadVec3(fp, &m->submeshes[i].vertices[j].tangent);
			SYS_FileReadVec3(fp, &m->submeshes[i].vertices[j].bitangent);

			DEBUG_Output("x: %f, y: %f, z: %f", m->submeshes[i].vertices[j].pos.x, m->submeshes[i].vertices[j].pos.y, m->submeshes[i].vertices[j].pos.z);
			DEBUG_Output("u: %f, v: %f", m->submeshes[i].vertices[j].uv.x, m->submeshes[i].vertices[j].uv.y);
		}
		SYS_FileReadUInt(fp, &m->submeshes[i].nindices);
		m->submeshes[i].indices = (unsigned int*)malloc(sizeof(unsigned int) * m->submeshes[i].nindices);

		for(unsigned int j = 0; j < m->submeshes[i].nindices; ++j)
			SYS_FileReadUInt(fp, &m->submeshes[i].indices[j]);
		m->submeshes[i].ntris = m->submeshes[i].nindices / 3;

		SYS_FileReadUInt(fp, &m->submeshes[i].ntextures);
		m->submeshes[i].textures = (gfx_texture**)malloc(sizeof(gfx_texture*) * m->submeshes[i].ntextures);

		char texturefilename[256];		
		for(unsigned int j = 0; j < m->submeshes[i].ntextures; ++j)
		{
			char fullpath[256] = FOLDER_MODELS;
			SYS_FileReadString(fp, texturefilename, 256);
			strcat_s(fullpath, 256, texturefilename);
			m->submeshes[i].textures[j] = GFX_LoadTexture(fullpath);
		}

		
		//should be based on shader
		m->submeshes[i].vbuffer = GFX_CreateVertexBuffer(m->submeshes[i].vertices, m->submeshes[i].nvertices, sizeof(scene_vertex), true);
		m->submeshes[i].ibuffer = GFX_CreateIndexBuffer(m->submeshes[i].indices, m->submeshes[i].nindices, true);
	}

	fclose(fp);

	return m;

	return 0;
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
		if(mesh->submeshes)
		{
			for(unsigned int i = 0; i < mesh->nsubmeshes; ++i)
			{				
				GFX_FreeVertexBuffer(mesh->submeshes[i].vbuffer);
				GFX_FreeIndexBuffer(mesh->submeshes[i].ibuffer);
			}
			free(mesh->submeshes);
			mesh->submeshes = 0;
		}
		

		free(mesh);
		mesh = 0;
	}
}

/******************************************************************/