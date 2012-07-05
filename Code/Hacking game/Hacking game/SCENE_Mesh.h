#ifndef SCENE_MESH_H_INCLUDED
#define SCENE_MESH_H_INCLUDED

#include "SYS_Math.h"

/******************************************************************/

struct scene_vertex
{
	math_vec3 pos;
	math_vec2 uv;
	math_vec3 normal;
	math_vec3 tangent;
	math_vec3 bitangent;
};

struct gfx_vertexbuffer;
struct gfx_indexbuffer;
struct gfx_texture;

struct scene_submesh
{	
	unsigned int ntris;
	math_matrix4 world;	
	unsigned int nvertices;
	scene_vertex * vertices;	
	unsigned int nindices;
	unsigned int * indices;
	int parent;
	gfx_vertexbuffer * vbuffer;
	gfx_indexbuffer * ibuffer;
	unsigned int ntextures;
	gfx_texture ** textures;
};

struct scene_mesh
{
	unsigned int nsubmeshes;
	scene_submesh * submeshes;
};

struct scene_meshinstance
{
	unsigned int index;
};

scene_mesh * SCENE_LoadMesh(const char * filename);

/******************************************************************/

#endif //SCENE_MESH_H_INCLUDED