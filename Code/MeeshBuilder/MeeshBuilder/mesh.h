#pragma once

#include "SYS_Math.h"

struct rgba
{
	float r,g,b,a;
};

struct vertex
{
	math_vec3 pos;
	math_vec2 uv;
	math_vec3 normal;
	math_vec3 tangent;
	math_vec3 bitangent;
};

struct material
{
	char name[128];
	rgba ambient;
	rgba diffuse;
	rgba specular;
	float shininess;
	float transparency;
};

struct submesh
{	
	math_matrix4 world;
	unsigned int nvertices;
	vertex * vertices;	
	unsigned int nindices;
	unsigned int * indices;
};

struct mesh
{
	unsigned int nsubmeshes;
	submesh * submeshes;
};

void MESH_ConvertMesh(const char * filename, const char * out);
mesh * MESH_LoadCollada(const char * filename);
void MESH_FreeMesh(mesh * m);
