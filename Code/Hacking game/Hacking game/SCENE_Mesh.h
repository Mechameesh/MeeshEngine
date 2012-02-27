#ifndef SCENE_MESH_H_INCLUDED
#define SCENE_MESH_H_INCLUDED

#include "SYS_Math.h"

/******************************************************************/

struct scene_uv
{
	float u;
	float v;
};

struct scene_mesh
{
	int ntris;
	int nvertices;
	math_vec3 * vertices;
	math_vec3 * normals;
	scene_uv * uv;
	int nindices;
	int * indices;
	int parent;
};

/******************************************************************/

#endif //SCENE_MESH_H_INCLUDED