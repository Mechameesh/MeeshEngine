#include "stdafx.h"
#include "mesh.h"
#include <fbxsdk.h>
#include "debug.h"

#ifdef _DEBUG
#pragma comment(lib, "fbxsdk-2013.1-mdd.lib")
#else
#pragma comment(lib, "fbxsdk-2013.1-md.lib")
#endif


static FbxManager * fbxmanager;

static void mesh_HandleTextureProperty(FbxProperty texproperty, int texindex, importmesh * imesh)
{
	if(texproperty.IsValid())
	{
		int ntextures = texproperty.GetSrcObjectCount(FbxTexture::ClassId);

		for(int i = 0; i < ntextures; ++i)
		{
			//check whether texture is layered or not. Discard layered textures for now

			FbxTexture * importtexture = FbxCast<FbxTexture>(texproperty.GetSrcObject(FbxTexture::ClassId, i));		

			texture texture;
			if(importtexture)
			{
				FbxFileTexture * filetexture = FbxCast<FbxFileTexture>(importtexture);				
				strcpy_s(texture.filename, 256, filetexture->GetRelativeFileName());				
				imesh->textures.push_back(texture);
			}
		}
	}
}

static void mesh_HandleMeshNode(FbxMesh * fmesh, vector<importmesh*> &imeshes)
{	
	importmesh * imesh = new importmesh();
	
	FbxGeometryConverter converter = FbxGeometryConverter(fbxmanager);

	FbxMesh * trimesh = converter.TriangulateMesh(fmesh);

	int i, j, k;
	
	FbxVector4 * controlpoints = trimesh->GetControlPoints();
	vertex v;
	FbxGeometryElementNormal * normalelement;
	FbxVector4 normal;
	
	//get vertices
	for(i = 0; i < trimesh->GetControlPointsCount(); ++i)
	{		
		memset(&v, 0, sizeof(vertex));
		v.pos.x = (float)controlpoints[i][0];
		v.pos.y = (float)controlpoints[i][1];
		v.pos.z = (float)controlpoints[i][2];		

		for(j = 0; j < trimesh->GetElementNormalCount(); ++j)
		{
			normalelement = trimesh->GetElementNormal(j);
			if(normalelement->GetMappingMode() == FbxGeometryElement::eByControlPoint && normalelement->GetReferenceMode() == FbxGeometryElement::eDirect)
			{
				normal = normalelement->GetDirectArray().GetAt(i);
				v.normal.x = (float)normal[0];
				v.normal.y = (float)normal[1];
				v.normal.z = (float)normal[2];
			}
		}
		
		imesh->vertices.push_back(v);
	}

	
	FbxVector2 uv;

	for(i = 0; i < trimesh->GetPolygonCount(); ++i)
	{
		for(j = 0; j < trimesh->GetPolygonSize(i); ++j)
		{
			int index = trimesh->GetPolygonVertex(i, j);
			imesh->indices.push_back(index);

			for(k = 0; k < trimesh->GetElementUVCount(); ++k)
			{
				FbxGeometryElementUV * uvelement = trimesh->GetElementUV(k);
				switch(uvelement->GetMappingMode())
				{
					case FbxGeometryElement::eByControlPoint:
					{
						switch(uvelement->GetReferenceMode())
						{
							case FbxGeometryElement::eDirect:
							{
								uv = uvelement->GetDirectArray().GetAt(index);
								imesh->vertices[index].uv.x = (float)uv[0];
								imesh->vertices[index].uv.y = (float)uv[1];
								break;
							}
							case FbxGeometryElement::eIndexToDirect:
							{
								int id = uvelement->GetIndexArray().GetAt(index);
								uv = uvelement->GetDirectArray().GetAt(id);
								imesh->vertices[index].uv.x = (float)uv[0];
								imesh->vertices[index].uv.y = (float)uv[1];
								break;
							}
							default:
								break;
						}
						break;
					}
					case FbxGeometryElement::eByPolygonVertex:
					{
						int textureUVindex = trimesh->GetTextureUVIndex(i, j);
						switch(uvelement->GetReferenceMode())
						{
							case FbxGeometryElement::eDirect:
							case FbxGeometryElement::eIndexToDirect:
							{
								uv = uvelement->GetDirectArray().GetAt(textureUVindex);
								imesh->vertices[index].uv.x = (float)uv[0];
								imesh->vertices[index].uv.y = (float)uv[1];
								break;
							}
							default:
								break;
						}						
						break;
					}
					case FbxGeometryElement::eByPolygon:
					case FbxGeometryElement::eAllSame:
					case FbxGeometryElement::eNone:
					default:
						break;
				}
			}
		}
	}

	//get indices and uvs
	/*int * indices = trimesh->GetPolygonVertices();	
	FbxGeometryElementUV * uvnode;
	uvnode = trimesh->GetElementUV(0);	//only use first set / no multiple uv mapping... for now...
	uv iuv;
		
	for(i = 0; i < trimesh->GetPolygonVertexCount(); ++i)
	{
		imesh->indices.push_back((unsigned int)indices[i]);
		
		if(uvnode)
		{
			iuv.u = (float)uvnode->GetDirectArray().GetAt(i).mData[0];
			iuv.v = (float)uvnode->GetDirectArray().GetAt(i).mData[1];
			imesh->uvs.push_back(iuv);
		}
	}*/

	//get textures
	int nmaterials = fmesh->GetNode()->GetSrcObjectCount(FbxSurfaceMaterial::ClassId);
	FbxSurfaceMaterial * curMaterial = 0;
	FbxProperty texproperty;

	for(i = 0; i < nmaterials; ++i)
	{		
		curMaterial = (FbxSurfaceMaterial *)fmesh->GetNode()->GetSrcObject(FbxSurfaceMaterial::ClassId, i);
		if(curMaterial)
		{
			int texindex;
			FBXSDK_FOR_EACH_TEXTURE(texindex)
			{
				texproperty = curMaterial->FindProperty(FbxLayerElement::sTextureChannelNames[texindex]);
				mesh_HandleTextureProperty(texproperty, texindex, imesh);
			}
		}
	}

	FbxAMatrix world = fmesh->GetNode()->EvaluateGlobalTransform();
	
	for(i = 0; i < 4; ++i)
		for(j = 0; j < 4; ++j)
			imesh->world.m[i][j] = (float)world.Get(i, j);

	imeshes.push_back(imesh);

	trimesh->Destroy();
}

void mesh_HandleNode(FbxNode * node, vector<importmesh*> &imeshes)
{	
	int i;

	if(node->GetNodeAttribute())
	{
		switch(node->GetNodeAttribute()->GetAttributeType())
		{
		case FbxNodeAttribute::eMesh:
			mesh_HandleMeshNode((FbxMesh*)node->GetNodeAttribute(), imeshes);
			break;			
		}
	}

	for(i = 0; i < node->GetChildCount(); ++i)
		mesh_HandleNode(node->GetChild(i), imeshes);
}

mesh * MESH_LoadFBX(const char * filename)
{
	fbxmanager = FbxManager::Create();

	if(!fbxmanager)
		return 0;

	FbxIOSettings *ios = FbxIOSettings::Create(fbxmanager, IOSROOT);		
	fbxmanager->SetIOSettings(ios);
		
	FbxImporter * importer = FbxImporter::Create(fbxmanager, "");

	if(!importer->Initialize(filename, -1, fbxmanager->GetIOSettings()))
		return 0;

	FbxScene * scene = FbxScene::Create(fbxmanager, "");

	importer->Import(scene);
	importer->Destroy();

	vector<importmesh *> imeshes = vector<importmesh *>();

	FbxNode * rootnode = scene->GetRootNode();
	if(rootnode)
		mesh_HandleNode(rootnode, imeshes);
	
	fbxmanager->Destroy();	

	mesh * m = MESH_ImportMeshesToMesh(&imeshes);
	
	for(unsigned int i = 0; i < imeshes.size(); ++i)
		delete imeshes[i];

	return m;
}
