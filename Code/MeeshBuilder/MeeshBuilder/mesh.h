#pragma once

#include "SYS_Math.h"
#include <vector>
#include <string>

using std::vector;
using std::string;

struct rgba
{
	float r,g,b,a;
};

struct texture
{	
	char filename[256];
};

struct vertex
{
	math_vec3 pos;
	math_vec3 normal;
	math_vec2 uv;
	math_vec3 tangent;
	math_vec3 bitangent;
};

struct material
{
	rgba ambient;
	rgba diffuse;
	rgba specular;
	float shininess;
	float transparency;
};

struct importmesh
{
	math_matrix4 world;
	vector<texture> textures;
	vector<vertex> vertices;
	vector<unsigned int> indices;
};

struct submesh
{		
	math_matrix4 world;
	unsigned int nvertices;
	vertex * vertices;
	unsigned int nindices;
	unsigned int * indices;
	unsigned int ntextures;	
	texture * textures;
};

struct mesh
{		
	unsigned int nsubmeshes;
	submesh * submeshes;
};

void MESH_ConvertMesh(const char * filename, const char * out);
mesh * MESH_LoadCollada(const char * filename);
mesh * MESH_LoadFBX(const char * filename);
void MESH_FreeMesh(mesh * m);
mesh * MESH_ImportMeshesToMesh(vector<importmesh*> * imeshes);