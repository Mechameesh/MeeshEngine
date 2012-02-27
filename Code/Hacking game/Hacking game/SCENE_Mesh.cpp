#include "SCENE_Mesh.h"
#include "SYS_File.h"
#include <string>

/******************************************************************/

scene_mesh * SCENE_LoadMesh(const char * filename)
{

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