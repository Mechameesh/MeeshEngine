#ifndef GFX_H_INCLUDED
#define GFX_H_INCLUDED
#include <D3D11.h>

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
	ID3D10Buffer * ibuffer;
};

struct gfx_texture
{
	int width;
	int height;
	int bpp;
	ID3D11ShaderResourceView * dxTexture;
};

bool GFX_Init(gfx_initsettings settings);
void GFX_Shutdown();
void GFX_SetViewport(int width, int height, float zmin, float zmax);

void GFX_ClearBuffer(int buffers);
void GFX_Swap();

/*********************************************************************/
gfx_vertexshader * GFX_LoadVertexShader(const char * filename, const char * entrypoint, gfx_vertexdesc * desc);
gfx_pixelshader * GFX_LoadPixelShader(const char * filename, const char * entrypoint);
/*********************************************************************/

gfx_texture * GFX_LoadTexture(const char * filename);
void GFX_FreeTexture(gfx_texture * texture);

/*********************************************************************/
gfx_vertexbuffer * GFX_CreateVertexBuffer(void * vertices, unsigned int nvertices, size_t stride, bool staticbuffer);
void GFX_FreeVertexBuffer(gfx_vertexbuffer * buffer);
void GFX_SetCurrentVertexBuffer(gfx_vertexbuffer * buffer, unsigned int offset);

void GFX_LockVertexBuffer(gfx_vertexbuffer * buffer, void ** data);
void GFX_UnlockVertexBuffer(gfx_vertexbuffer * buffer);

/*********************************************************************/
void GFX_EnableBuffer(int buffer, bool enable);
void GFX_SetPrimitiveType(int type);
void GFX_SetTexture(gfx_texture * tex, int level);
/*********************************************************************/
void GFX_DrawPrimitives(gfx_vertexshader * vshader, gfx_pixelshader * pshader, unsigned int nverts);
/*********************************************************************/


#endif //GFX_H_INCLUDED