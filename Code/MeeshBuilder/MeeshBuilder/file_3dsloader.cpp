#include "stdafx.h"
#include "file.h"
#include "mesh.h"

#if 0
enum 
{
	// Primary chunk
	 M3DS_MAIN3DS = 0x4D4D,

	// Main Chunks
	 M3DS_EDIT3DS = 0x3D3D,
	 M3DS_KEYF3DS = 0xB000,
	 M3DS_VERSION = 0x0002,
	 M3DS_MESHVERSION = 0x3D3E,

	// sub chunks of M3DS_EDIT3DS
	 M3DS_EDIT_MATERIAL = 0xAFFF,
	 M3DS_EDIT_OBJECT   = 0x4000,

	// sub chunks of M3DS_EDIT_MATERIAL
	 M3DS_MATNAME       = 0xA000,
	 M3DS_MATAMBIENT    = 0xA010,
	 M3DS_MATDIFFUSE    = 0xA020,
	 M3DS_MATSPECULAR   = 0xA030,
	 M3DS_MATSHININESS  = 0xA040,
	 M3DS_MATSHIN2PCT   = 0xA041,
	 M3DS_TRANSPARENCY  = 0xA050,
	 M3DS_TRANSPARENCY_FALLOFF  = 0xA052,
	 M3DS_REFL_BLUR     = 0xA053,
	 M3DS_TWO_SIDE      = 0xA081,
	 M3DS_WIRE          = 0xA085,
	 M3DS_SHADING       = 0xA100,
	 M3DS_MATTEXMAP     = 0xA200,
	 M3DS_MATSPECMAP    = 0xA204,
	 M3DS_MATOPACMAP    = 0xA210,
	 M3DS_MATREFLMAP    = 0xA220,
	 M3DS_MATBUMPMAP    = 0xA230,
	 M3DS_MATMAPFILE    = 0xA300,
	 M3DS_MAT_TEXTILING = 0xA351,
	 M3DS_MAT_USCALE    = 0xA354,
	 M3DS_MAT_VSCALE    = 0xA356,
	 M3DS_MAT_UOFFSET   = 0xA358,
	 M3DS_MAT_VOFFSET   = 0xA35A,

	// subs of M3DS_EDIT_OBJECT
	 M3DS_OBJTRIMESH    = 0x4100,

	// subs of M3DS_OBJTRIMESH
	 M3DS_TRIVERT       = 0x4110,
	 M3DS_POINTFLAGARRAY= 0x4111,
	 M3DS_TRIFACE       = 0x4120,
	 M3DS_TRIFACEMAT    = 0x4130,
	 M3DS_TRIUV         = 0x4140,
	 M3DS_TRISMOOTH     = 0x4150,
	 M3DS_TRIMATRIX     = 0x4160,
	 M3DS_MESHCOLOR     = 0x4165,
	 M3DS_DIRECT_LIGHT  = 0x4600,
	 M3DS_DL_INNER_RANGE= 0x4659,
	 M3DS_DL_OUTER_RANGE= 0x465A,
	 M3DS_DL_MULTIPLIER = 0x465B,
	 M3DS_CAMERA        = 0x4700,
	 M3DS_CAM_SEE_CONE  = 0x4710,
	 M3DS_CAM_RANGES    = 0x4720,

	// subs of M3DS_KEYF3DS
	 M3DS_KF_HDR        = 0xB00A,
	 M3DS_AMBIENT_TAG   = 0xB001,
	 M3DS_OBJECT_TAG    = 0xB002,
	 M3DS_CAMERA_TAG    = 0xB003,
	 M3DS_TARGET_TAG    = 0xB004,
	 M3DS_LIGHTNODE_TAG = 0xB005,
	 M3DS_KF_SEG        = 0xB008,
	 M3DS_KF_CURTIME    = 0xB009,
	 M3DS_KF_NODE_HDR   = 0xB010,
	 M3DS_PIVOTPOINT    = 0xB013,
	 M3DS_BOUNDBOX      = 0xB014,
	 M3DS_MORPH_SMOOTH  = 0xB015,
	 M3DS_POS_TRACK_TAG = 0xB020,
	 M3DS_ROT_TRACK_TAG = 0xB021,
	 M3DS_SCL_TRACK_TAG = 0xB022,
	 M3DS_NODE_ID       = 0xB030,

	// Viewport definitions
	 M3DS_VIEWPORT_LAYOUT = 0x7001,
	 M3DS_VIEWPORT_DATA   = 0x7011,
	 M3DS_VIEWPORT_DATA_3 = 0x7012,
	 M3DS_VIEWPORT_SIZE   = 0x7020,

	// different color chunk types
	 M3DS_COL_RGB    = 0x0010,
	 M3DS_COL_TRU    = 0x0011,
	 M3DS_COL_LIN_24 = 0x0012,
	 M3DS_COL_LIN_F  = 0x0013,

	// percentage chunk types
	 M3DS_PERCENTAGE_I = 0x0030,
	 M3DS_PERCENTAGE_F = 0x0031,

	 M3DS_CHUNK_MAX		= 0xFFFF
};

#pragma pack(push, packing)
#pragma pack(1)

struct chunkheader
{
	unsigned short id;
	int length;
};

#pragma pack(pop, packing)

struct chunkdata
{
	chunkheader header;
	int read;
};

static void SkipChunk(FILE * fp, chunkdata * data)
{
	int offset = data->header.length - data->read;
	fseek(fp, offset, SEEK_CUR);
	data->read += offset;
}

static bool ReadColourChunk(FILE * fp, chunkdata * parent, rgba * colour)
{
	chunkdata data;
	ReadChunkHeader(fp, &data);

	unsigned char c[3];
	float cf[3];

	switch(data.header.id)
	{
	case M3DS_COL_TRU:
	case M3DS_COL_LIN_24:
		{
			fread(c, sizeof(char), 3, fp);
			colour->r = (float)c[0] / 255.0f;
			colour->g = (float)c[1] / 255.0f;
			colour->b = (float)c[2] / 255.0f;
			colour->a = 1.0f;
			data.read += sizeof(char) * 3;
		} break;
	case M3DS_COL_RGB:
	case M3DS_COL_LIN_F:
		{
			FILE_ReadFloat(fp, &colour->r);
			FILE_ReadFloat(fp, &colour->g);
			FILE_ReadFloat(fp, &colour->b);
			colour->a = 1.0f;

			data.read += sizeof(float) * 3;
		} break;
	default:
		{
			//unknown colour chunk
			fseek(fp, data.header.length - data.read, SEEK_CUR);
			data.read += data.header.length - data.read;
		} break;
	}

	parent->read += data.read;

	return true;
}

static bool ReadPercentageChunk(FILE * fp, chunkdata * parent, float * percentage)
{
	chunkdata data;
	ReadChunkHeader(fp, &data);

	short spercentage;

	switch(data.header.id)
	{
	case M3DS_PERCENTAGE_I:
		{
			FILE_ReadShort(fp, &spercentage);
			*percentage = spercentage / 100.0f;
			data.read += sizeof(short);
		} break;
	case M3DS_PERCENTAGE_F:
		{
			FILE_ReadFloat(fp, percentage);
			data.read += sizeof(float);
		}break;
	default:
		{
			//unknown percentage chunk
			fseek(fp, data.header.length - data.read, SEEK_CUR);
			data.read += data.header.length - data.read;
		} break;
	}

	parent->read += data.read;

	return true;
}

static void ReadIndices(mesh *m, FILE *fp, chunkdata *data)
{
	unsigned short nfaces;
	FILE_ReadUShort(fp, &nfaces);
	data->read += sizeof(unsigned short);
	
	//each face has 3 indices + edge flag
	m->nindices = nfaces * 3;
	int indexbuffersize = m->nindices * sizeof(unsigned short);	
	int index = 0;
	int pos = 0;
	m->indices = (unsigned short *)malloc(indexbuffersize);
	unsigned short edgeflag;
	while(index < nfaces * 4)
	{
		FILE_ReadUShort(fp, &m->indices[pos++]);
		FILE_ReadUShort(fp, &m->indices[pos++]);
		FILE_ReadUShort(fp, &m->indices[pos++]);
		FILE_ReadUShort(fp, &edgeflag);
		index += 4;
	}

	data->read += nfaces * sizeof(unsigned short) * 4;
}

static void ReadVertices(mesh *m, FILE * fp, chunkdata * data)
{	
	FILE_ReadUShort(fp, &m->nvertices);

	data->read += sizeof(unsigned short);

	const int vbuffersize = m->nvertices * sizeof(vec3);

	if(data->header.length - data->read != vbuffersize)
		return;

	m->vertices = (vec3*)malloc(vbuffersize);

	fread(m->vertices, vbuffersize, 1, fp);

	data->read += vbuffersize;
}

static void ReadChunkHeader(FILE * fp, chunkdata * data)
{
	fread(&data->header, sizeof(chunkheader), 1, fp);
	data->read += sizeof(chunkheader);
}

static bool ReadFrameChunk(mesh * m, FILE * fp, chunkdata * parent)
{
	chunkdata data;
	ReadChunkHeader(fp, &data);
	if(data.header.id != M3DS_KF_HDR)
		return false;
	else
	{
		unsigned short version;
		FILE_ReadUShort(fp, &version);

	}
	return true;
}

static bool ReadMaterialChunk(mesh * m, FILE * fp, chunkdata * parent)
{
	material mat;
	
	while(parent->read < parent->header.length)
	{
		chunkdata data;
		ReadChunkHeader(fp, &data);

		switch(data.header.id)
		{
			case M3DS_MATNAME:
			{
				char * c = (char*)malloc(data.header.length - data.read);
				fread(c, 1, data.header.length - data.read, fp);

				if(strlen(c))
					int x = 0;

				data.read += data.header.length - data.read;
				free(c);
			} break;
			case M3DS_MATAMBIENT:
				ReadColourChunk(fp, &data, &material);
				break;


			default:
				SkipChunk(fp, &data);
				break;
		}
		parent->read += data.read;
	}
	return true;
}

static bool readObjectChunk(mesh * m, FILE * fp, chunkdata * parent)
{
	while(parent->read < parent->header.length)
	{
		chunkdata data;
		ReadChunkHeader(fp, &data);

		switch(data.header.id)
		{
		case M3DS_OBJTRIMESH:
			readObjectChunk(m, fp, &data);
			break;

		case M3DS_TRIVERT:
			ReadVertices(m, fp, &data);
			break;
		case M3DS_POINTFLAGARRAY:
			SkipChunk(fp, &data);
			break;
		case M3DS_TRIFACE:
			ReadIndices(m, fp, &data);
			readObjectChunk(m, fp, &data);
			break;
		case M3DS_TRIFACEMAT:
			SkipChunk(fp, &data);
			break;
		case M3DS_TRIUV:
			SkipChunk(fp, &data);
			break;
		case M3DS_TRIMATRIX:
			SkipChunk(fp, &data);
			break;
		case M3DS_MESHCOLOR:
			SkipChunk(fp, &data);
			break;
		case M3DS_TRISMOOTH:
			SkipChunk(fp, &data);
			break;
		default:
			SkipChunk(fp, &data);
			break;
		}
		parent->read += data.read;
	}

	return true;
}

static bool ReadChunk(mesh * m, FILE * fp, chunkdata * parent)
{
	while(parent->read < parent->header.length)
	{
		chunkdata data;
		ReadChunkHeader(fp, &data);

		switch(data.header.id)
		{
			case M3DS_VERSION:
			{
				unsigned short version;
				FILE_ReadUShort(fp, &version);
				fseek(fp, data.header.length - data.read - 2, SEEK_CUR);
				data.read += data.header.length - data.read;
			} break;
			case M3DS_EDIT_MATERIAL:
				//ReadMaterialChunk(m, fp, &data);
				SkipChunk(fp, &data);
				break;
			case M3DS_KEYF3DS:
				//ReadFrameChunk(m, fp, &data);
				SkipChunk(fp, &data);
			case M3DS_EDIT3DS:
				break;
			case M3DS_MESHVERSION:
			case 0x01:
				{
					unsigned int version;
					FILE_ReadUInt(fp, &version);
					data.read += sizeof(unsigned int);
				} break;
			case M3DS_EDIT_OBJECT:
				{
					char buffer[32];
					int pos = 0;
					buffer[0] = 1;
					while(buffer[pos - 1] && pos < 32)
						FILE_ReadChar(fp, &buffer[pos++]);
					readObjectChunk(m, fp, &data);
				} break;
			default:
				SkipChunk(fp, &data);
				break;
		}
		parent->read += data.read;
	}

	return true;
}

mesh * Load3DS(const char * filename)
{
	FILE * fp = fopen(filename, "rb");
	if(!fp)
	{
		printf("Failed to open \"%s\"", filename);
		return 0;
	}

	chunkdata data;

	ReadChunkHeader(fp, &data);
	

	if(data.header.id != M3DS_MAIN3DS)
	{
		printf("Wrong file header in \"%s\"", filename);
		fclose(fp);
		return 0;
	}	

	mesh * m = (mesh *)malloc(sizeof(mesh));
	memset(m, 0, sizeof(mesh));	

	if(ReadChunk(m, fp, &data))
	{

	}
	else
		MESH_FreeMesh(m);		

	fclose(fp);
	return m;
}

#endif