#ifndef GFX_H_INCLUDED
#define GFX_H_INCLUDED
#include <D3D11.h>

#include "SCENE_Mesh.h"

/*********************************************************************/
enum
{
	GFX_BUFFER_NONE		= 0,
	GFX_BUFFER_COLOUR	= 1,
	GFX_BUFFER_DEPTH	= 1 << 1,
	GFX_BUFFER_STENCIL	= 1 << 2,
};

enum
{
	GFX_VERTEXFORMAT_RG_FLOAT,	
	GFX_VERTEXFORMAT_RG_UINT,
	GFX_VERTEXFORMAT_RGB_FLOAT,	
	GFX_VERTEXFORMAT_RGB_UINT,
	GFX_VERTEXFORMAT_RGBA_FLOAT,
	GFX_VERTEXFORMAT_RGBA_UINT	
};

enum
{
	GFX_PRIMITIVETYPE_TRIANGLELIST
};

enum gfx_shadertype
{
	GFX_SHADER_VS,
	GFX_SHADER_PS
};

enum
{
	GFX_FILTER_MIN_MAG_MIP_POINT,
	GFX_FILTER_MIN_MAG_POINT_MIP_LINEAR,
	GFX_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT,
	GFX_FILTER_MIN_POINT_MAG_MIP_LINEAR,
	GFX_FILTER_MIN_MAG_MIP_LINEAR,
	GFX_FILTER_ANISOTROPIC,
	GFX_FILTER_COMPARISON_ANISOTROPIC,
};

enum
{
	GFX_TEXTURE_ADDRESS_WRAP,
	GFX_TEXTURE_ADDRESS_MIRROR,
	GFX_TEXTURE_ADDRESS_CLAMP,
	GFX_TEXTURE_ADDRESS_BORDER,
	GFX_TEXTURE_ADDRESS_MIRROR_ONCE	
};

enum
{
	GFX_COLOUR_WHITE,
	GFX_COLOUR_RED,
	GFX_COLOUR_GREEN,
	GFX_COLOUR_BLUE,
	GFX_COLOUR_BLACK,
	GFX_NCOLOURS
};

/*********************************************************************/


struct gfx_colour
{
	float r,g,b,a;
};

struct gfx_initsettings
{	
	int width;
	int height;
	gfx_colour clearcolour;
	bool fullscreen;
	bool vsync;
};

struct gfx_vertexshader
{
	ID3D11VertexShader * shader;
	ID3D11InputLayout * layout;
};

struct gfx_pixelshader
{
	ID3D11PixelShader * shader;
};

struct gfx_vertexdesc
{
	char semantic[32];
	int vertexformat;	
};

struct gfx_vertexbuffer
{
	ID3D11Buffer * vbuffer;
	size_t stride;
};

struct gfx_indexbuffer
{
	ID3D11Buffer * ibuffer;
};

struct gfx_constantbuffer
{
	ID3D11Buffer * cbuffer;
};

struct gfx_texture
{
	int width;
	int height;	
	ID3D11ShaderResourceView * dxTexture;
};

struct gfx_samplerstate
{
	ID3D11SamplerState * state;
};

struct gfx_uv
{
	float u0;	
	float v0;
	float u1;
	float v1;	
};

struct gfx_font
{
	gfx_texture * texture;
	int ncharacters;
	char * characters;
	gfx_uv * uvs;
	unsigned char * advances;
};

bool GFX_Init(gfx_initsettings settings);
void GFX_Shutdown();
void GFX_SetViewport(int width, int height, float zmin, float zmax);

void GFX_SetClearColour(const gfx_colour * cc);

void GFX_ClearBuffer(int buffers);
void GFX_Swap();

/*********************************************************************/

int GFX_GetXRes();
int GFX_GetYRes();

/*********************************************************************/

gfx_vertexshader * GFX_LoadVertexShader(const char * filename, const char * entrypoint, gfx_vertexdesc * desc, unsigned int nsemantics);
gfx_pixelshader * GFX_LoadPixelShader(const char * filename, const char * entrypoint);

/*********************************************************************/

void GFX_SetVertexShader(gfx_vertexshader * shader);
void GFX_SetPixelShader(gfx_pixelshader * shader);

/*********************************************************************/

gfx_texture * GFX_LoadTexture(const char * filename);
void GFX_FreeTexture(gfx_texture * texture);

/*********************************************************************/
gfx_vertexbuffer * GFX_CreateVertexBuffer(void * vertices, unsigned int nvertices, size_t stride, bool staticbuffer);
void GFX_FreeVertexBuffer(gfx_vertexbuffer * buffer);
void GFX_SetCurrentVertexBuffer(gfx_vertexbuffer * buffer, unsigned int offset);

void GFX_LockVertexBufferWrite(gfx_vertexbuffer * buffer, void ** data);
void GFX_LockVertexBufferRead(gfx_vertexbuffer * buffer, void **data);
void GFX_UnlockVertexBuffer(gfx_vertexbuffer * buffer);

/*********************************************************************/

gfx_indexbuffer * GFX_CreateIndexBuffer(void * indices, unsigned int nindices, bool staticbuffer);
void GFX_FreeIndexBuffer(gfx_indexbuffer * buffer);
void GFX_SetCurrentIndexBuffer(gfx_indexbuffer * buffer, unsigned int offset);

void GFX_LockIndexBuffer(gfx_indexbuffer * buffer, void ** data);
void GFX_UnlockVertexBuffer(gfx_indexbuffer * buffer);

/*********************************************************************/
gfx_constantbuffer * GFX_CreateConstantBuffer(unsigned int size);
void GFX_SetVertexConstantBuffer(gfx_constantbuffer * cb);
void GFX_SetPixelConstantBuffer(gfx_constantbuffer * cb);
void GFX_UpdateConstantBuffer(gfx_constantbuffer * cb, const void * data);
void GFX_FreeConstantBuffer(gfx_constantbuffer * buffer);
/*********************************************************************/
gfx_samplerstate * GFX_CreateSamplerState(int filter, int addressU, int addressV, int addressW);
void GFX_FreeSamplerState(gfx_samplerstate * state);
/*********************************************************************/
void GFX_EnableBuffer(int buffer, bool enable);
void GFX_SetPrimitiveType(int type);
void GFX_SetTexture(const gfx_texture * tex, int level);
void GFX_SetSamplerState(int index, gfx_samplerstate * state);
/*********************************************************************/
void GFX_DrawPrimitives(unsigned int nverts);
void GFX_DrawPrimitivesIndexed(unsigned int nindices);
/*********************************************************************/

const gfx_colour * GFX_GetColour(int colour);

/*********************************************************************/

gfx_font * GFX_LoadFont(const char * filename);
void GFX_FreeFont(gfx_font * font);
void GFX_SetFont(gfx_font * font);
void GFX_SetTextPos2D(float x, float y);
void GFX_SetTextPos2Dabs(int x, int y);
void GFX_SetTextPos3D(const math_matrix4 * world);
void GFX_SetLineHeight(int height);
void GFX_SetCharacterSpace(int space);
void GFX_SetTextSize(float x, float y);
void GFX_SetTextColour(gfx_colour * colour);
void GFX_PrintText2D(char * string, ...);

/*********************************************************************/


#endif //GFX_H_INCLUDED