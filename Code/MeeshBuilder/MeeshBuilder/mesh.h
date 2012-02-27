#pragma once

struct vec3
{
	float x, y ,z;
};

struct uv
{
	float u, v;
};

struct mesh
{
	int ntris;
	int nvertices;
	vec3 * vertices;
	vec3 * normals;
	uv * uv;
	int nindices;
	int * indices;
	int parent;
};

void Convert3DS(const char * filename);