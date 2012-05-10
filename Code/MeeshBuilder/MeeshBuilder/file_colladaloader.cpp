#include "stdafx.h"
#include "mesh.h"
#include <dae.h>
#include <1.5\dom\domCOLLADA.h>
#include <vector>

#pragma comment(lib, "collada15dom2-vc100-mt.lib")

/******************************************************/

using std::vector;
using std::string;
using std::stringstream;

/******************************************************/



struct colladamesh
{
	int ntris;	

	vector<math_vec3> positions;
	vector<math_vec2> uvs;
	vector<math_vec3> normals;
	vector<math_vec3> tangents;
	vector<math_vec3> bitangents;

	math_matrix4 world;
	vector<unsigned int> indices;
};

/******************************************************/

static DAE dae;
static domCOLLADA * root;

/******************************************************/

static void mesh_ProcessMatrix(math_matrix4 * dest, daeElement * e)
{
	//read as (RH)opengl matrix
	std::string matrix = e->getCharData();	
	std::stringstream stm(matrix);

	stm >> dest->m[0][0];
	stm >> dest->m[0][1];
	stm >> dest->m[0][2];
	stm >> dest->m[0][3];

	stm >> dest->m[1][0];
	stm >> dest->m[1][1];
	stm >> dest->m[1][2];
	stm >> dest->m[1][3];

	stm >> dest->m[2][0];
	stm >> dest->m[2][1];
	stm >> dest->m[2][2];
	stm >> dest->m[2][3];

	stm >> dest->m[3][0];
	stm >> dest->m[3][1];
	stm >> dest->m[3][2];
	stm >> dest->m[3][3];
}

/******************************************************/

static void mesh_ProcessSources(colladamesh * m, daeElement * source)
{
	//positions
	if(source->getAttribute("name").find("position") != string::npos)
	{
		daeElement * float_array = source->getChild("float_array");

		unsigned int count = atoi(float_array->getAttribute("count").data());

		string positionArray = float_array->getCharData();

		stringstream stm(positionArray);

		for(unsigned int i = 0; i < (count / 3); ++i)
		{
			math_vec3 pos;

			stm >> pos.x;
			stm >> pos.y;
			stm >> pos.z;

			m->positions.push_back(pos);
		}
		return;
	}

	//UVs
	if(source->getAttribute("name").find("map1") != string::npos)
	{
		daeElement* float_array = source->getChild("float_array");

		unsigned int count = atoi(float_array->getAttribute("count").data());

		string uvArray = float_array->getCharData();

		stringstream stm(uvArray);

		for(unsigned int i = 0; i < (count / 2); ++i)
		{
			math_vec2 uv;

			stm >> uv.x;
			stm >> uv.y;

			m->uvs.push_back(uv);
		}
		return;
	}

	//normals
	if(source->getAttribute("name").find("normal") != string::npos)
	{
		daeElement* float_array = source->getChild("float_array");

		unsigned int count = atoi(float_array->getAttribute("count").data());

		string normalArray = float_array->getCharData();

		stringstream stm(normalArray);

		for(unsigned int i = 0; i < (count / 3); ++i)
		{
			math_vec3 normal;

			stm >> normal.x;
			stm >> normal.y;
			stm >> normal.z;

			m->normals.push_back(normal);
		}
		return;
	}

	//tangents
	if(source->getAttribute("id").find("map1-tangents") != string::npos || source->getAttribute("id").find("textangents") != string::npos)
	{
		daeElement* float_array = source->getChild("float_array");

		unsigned int count = atoi(float_array->getAttribute("count").data());

		string tangentArray = float_array->getCharData();

		stringstream stm(tangentArray);

		for(unsigned int i = 0; i < (count / 3); ++i)
		{
			math_vec3 tangent;

			stm >> tangent.x;
			stm >> tangent.y;
			stm >> tangent.z;

			m->tangents.push_back(tangent);
		}
		return;
	}

	//tangents
	if(source->getAttribute("id").find("map1-binormals") != string::npos || source->getAttribute("id").find("texbinormals") != string::npos)
	{
		daeElement* float_array = source->getChild("float_array");

		unsigned int count = atoi(float_array->getAttribute("count").data());

		string bitangentArray = float_array->getCharData();

		stringstream stm(bitangentArray);

		for(unsigned int i = 0; i < (count / 3); ++i)
		{
			math_vec3 bitangent;

			stm >> bitangent.x;
			stm >> bitangent.y;
			stm >> bitangent.z;

			m->bitangents.push_back(bitangent);
		}
		return;
	}
}

/******************************************************/
static void mesh_ProcessTriangles(colladamesh * m, daeElement * triangles)
{
	daeElement *p = triangles->getDescendant("p");

	unsigned int count = atoi(triangles->getAttribute("count").data()) * 3;

	string pArray = p->getCharData();

	stringstream stm(pArray);

	for(unsigned int i = 0; i < count; ++i);
	{
		unsigned int index = 0;
		stm >> index;
		m->indices.push_back(index);
	}
}

/******************************************************/

static void mesh_ProcessVisualScenes(vector<colladamesh*> &meshes, vector<daeElement*> geometries, daeElement * lib_vs)
{
	daeElement * scene = lib_vs->getDescendant("visual_scene");

	//get all child nodes of visual scene
	daeTArray<daeElementRef> nodes = scene->getChildren();
	
	for(unsigned int i = 0; i < nodes.getCount(); ++i)
	{
		domInstance_geometry * instance_geometry = 0;
		instance_geometry = (domInstance_geometry*)nodes[i]->getDescendant("instance_geometry");

		if(nodes[i]->getAttribute("type").data() == "JOINT")
			continue;

		if(!instance_geometry)
			continue;

		daeElement * geometry = instance_geometry->getUrl().getElement();

		if(!geometry)
			continue;		

		colladamesh * m = (colladamesh *)malloc(sizeof(mesh));
		memset(m, 0, sizeof(colladamesh));		
		mesh_ProcessMatrix(&m->world, nodes[i]->getDescendant("matrix"));
		meshes.push_back(m);
		geometries.push_back(geometry);
	}
}

/******************************************************/

static void mesh_ProcessGeometries(vector<colladamesh*> &meshes, std::vector<daeElement*> geometries, daeElement * lib_geometries)
{
	for(unsigned int i = 0; i < meshes.size(); ++i)
	{
		daeElement * mesh = geometries[i]->getDescendant("mesh");
		daeTArray<daeElementRef> sources = mesh->getChildren();

		daeElement * triangles = mesh->getDescendant("triangles");

		for(unsigned int j = 0; j < sources.getCount(); ++j)
			mesh_ProcessSources(meshes[i], sources[j]);

		mesh_ProcessTriangles(meshes[i], triangles);
	}
}

/******************************************************/

mesh * MESH_LoadCollada(const char * filename)
{
	root = dae.open(filename);

	vector<colladamesh*> meshes = vector<colladamesh*>();
	vector<daeElement*> geometries = vector<daeElement*>();

	if(!root)
		return 0;

	daeElement * visualscenes = root->getDescendant("library_visual_scenes");
	if(!visualscenes)
		return 0;

	daeElement * lib_geometries = root->getDescendant("library_geometries");

	if(lib_geometries)
		return 0;

	mesh_ProcessVisualScenes(meshes, geometries, visualscenes);
	mesh_ProcessGeometries(meshes, geometries, lib_geometries);

	mesh * m = (mesh*)malloc(sizeof(mesh));
	memset(m, 0, sizeof(mesh));

	m->nsubmeshes = meshes.size();
	m->submeshes = (submesh*)malloc(sizeof(submesh) * m->nsubmeshes);
	memset(m->submeshes, 0, sizeof(submesh) * m->nsubmeshes);
			
	for(unsigned int i = 0; i < meshes.size(); ++i)
	{
		m->submeshes[i].world = meshes[i]->world;

		m->submeshes[i].nvertices = meshes[i]->positions.size();
		m->submeshes[i].vertices = (vertex*)malloc(sizeof(vertex) * m->submeshes[i].nvertices);
		memset(m->submeshes[i].vertices, 0, sizeof(sizeof(vertex) * m->submeshes[i].nvertices));

		//cache trashing / can be cleaned
		for(unsigned int j = 0; j < m->submeshes[i].nvertices; ++j)
		{
			m->submeshes[i].vertices[j].pos = meshes[i]->positions[j];
			m->submeshes[i].vertices[j].uv = meshes[i]->uvs[j];
			m->submeshes[i].vertices[j].normal = meshes[i]->normals[j];
			m->submeshes[i].vertices[j].tangent = meshes[i]->tangents[j];
			m->submeshes[i].vertices[j].bitangent = meshes[i]->bitangents[j];
		}

		m->submeshes[i].nindices = meshes[i]->indices.size();
		m->submeshes[i].indices = (unsigned int*)malloc(sizeof(vertex) * m->submeshes[i].nindices);
		memset(m->submeshes[i].indices, 0, sizeof(vertex) * m->submeshes[i].nindices);

		for(unsigned int j = 0; j < m->submeshes[i].nindices; ++j)
		{
			m->submeshes[i].indices[j] = meshes[i]->indices[j];
		}	

		free(meshes[i]);
		meshes[i] = 0;
	}	

	dae.close(filename);

	return m;
}