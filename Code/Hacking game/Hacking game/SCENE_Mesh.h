#ifndef SCENE_MESH_H_INCLUDED
#define SCENE_MESH_H_INCLUDED

#include "SYS_Math.h"

/******************************************************************/

struct scene_uv
{
	float u;
	float v;
};

struct gfx_vertexbuffer;
struct gfx_indexbuffer;

struct scene_mesh
{
	unsigned int ntris;
	unsigned int nvertices;
	math_vec3 * vertices;
	math_vec3 * normals;
	scene_uv * uv;
	unsigned int nindices;
	unsigned int * indices;
	int parent;
	gfx_vertexbuffer * vbuffer;
	gfx_indexbuffer * ibuffer;

};

scene_mesh * SCENE_LoadMesh(const char * filename);

/******************************************************************/

#endif //SCENE_MESH_H_INCLUDED