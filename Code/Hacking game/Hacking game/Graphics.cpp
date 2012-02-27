#include <Windows.h>
#include <d3d11.h>
#include <d3dX11.h>
#include <DxErr.h>
#include <d3dcompiler.h>
#include <string.h>
#include <stdio.h>
#include "Graphics.h"

#define DISPLAYERRORMESSAGE(x) if(FAILED(x)) {const char * error = DXGetErrorDescription(x); MessageBox(0, error, "DXError", MB_OK);}

extern HINSTANCE hInstance;
extern HWND hWnd;

static D3D_FEATURE_LEVEL gfx_devicefeatures = D3D_FEATURE_LEVEL_11_0;
static IDXGISwapChain * gfx_swapchain;
static ID3D11DeviceContext * gfx_devicecontext;
static ID3D11Device * gfx_device;
static ID3D11RenderTargetView * gfx_rendertargetview;
static ID3D11Texture2D * gfx_depthstencilbuffer;
static ID3D11DepthStencilView * gfx_depthstencilview;
static ID3D11DepthStencilState * gfx_depthon;
static ID3D11DepthStencilState * gfx_depthoff;
static ID3D11RasterizerState * gfx_renderstate;

static bool gfx_vsync = false;

static gfx_colour gfx_clearcolour = {0.0f, 0.0f, 0.0f, 1.0f};

/*********************************************************************/

inline static bool gfx_CheckDXError(HRESULT hr)
{
	if(FAILED(hr))
	{
		MessageBox(0, DXGetErrorDescription(hr), "ERROR", MB_OK | MB_ICONERROR);
		return false;
	}
	return true;
}

/*********************************************************************/

bool GFX_Init(gfx_initsettings settings)
{
	DXGI_SWAP_CHAIN_DESC swapdesc;
	ZeroMemory(&swapdesc, sizeof(swapdesc));

	swapdesc.BufferCount = 1;
	swapdesc.BufferDesc.Width = settings.width;
	swapdesc.BufferDesc.Height = settings.height;
	swapdesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapdesc.BufferDesc.RefreshRate.Numerator = 60;
	swapdesc.BufferDesc.RefreshRate.Denominator = 1;
	swapdesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapdesc.OutputWindow = hWnd;
	swapdesc.SampleDesc.Count = 1;
	swapdesc.SampleDesc.Quality = 0;
	swapdesc.Windowed = !settings.fullscreen;

	HRESULT hr;

	hr = D3D11CreateDeviceAndSwapChain(0, D3D_DRIVER_TYPE_HARDWARE, 0, 0, 0, 0, D3D11_SDK_VERSION, &swapdesc, &gfx_swapchain, &gfx_device, &gfx_devicefeatures, &gfx_devicecontext);
	if(!gfx_CheckDXError(hr))
		return false;

	//create render target view
	ID3D11Texture2D * backbuffer;
	hr = gfx_swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backbuffer);
	if(!gfx_CheckDXError(hr))
		return false;
	
	hr = gfx_device->CreateRenderTargetView(backbuffer, 0, &gfx_rendertargetview);
	backbuffer->Release();
	if(!gfx_CheckDXError(hr))
		return false;

	D3D11_TEXTURE2D_DESC depthbufferdesc;
	ZeroMemory(&depthbufferdesc, sizeof(depthbufferdesc));

	depthbufferdesc.Width = settings.width;
	depthbufferdesc.Height = settings.height;
	depthbufferdesc.MipLevels = 1;
	depthbufferdesc.ArraySize = 1;
	depthbufferdesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthbufferdesc.SampleDesc.Count = 1;
	depthbufferdesc.SampleDesc.Quality = 0;
	depthbufferdesc.Usage = D3D11_USAGE_DEFAULT;
	depthbufferdesc.BindFlags =	D3D11_BIND_DEPTH_STENCIL;
	depthbufferdesc.CPUAccessFlags = 0;
	depthbufferdesc.MiscFlags = 0;

	hr = gfx_device->CreateTexture2D(&depthbufferdesc, 0, &gfx_depthstencilbuffer);
	if(!gfx_CheckDXError(hr))
		return false;
		
	D3D11_DEPTH_STENCIL_DESC depthstencildesc;
	ZeroMemory(&depthstencildesc, sizeof(depthstencildesc));
	
	depthstencildesc.DepthEnable = true;
	depthstencildesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthstencildesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthstencildesc.StencilEnable = true;
	depthstencildesc.StencilReadMask = 0xFF;
	depthstencildesc.StencilWriteMask = 0xFF;

	depthstencildesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthstencildesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthstencildesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthstencildesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	depthstencildesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthstencildesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthstencildesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthstencildesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	hr = gfx_device->CreateDepthStencilState(&depthstencildesc, &gfx_depthon);
	if(!gfx_CheckDXError(hr))
		return false;

	gfx_devicecontext->OMSetDepthStencilState(gfx_depthon, 1);

	ZeroMemory(&depthstencildesc, sizeof(depthstencildesc));
	
	depthstencildesc.DepthEnable = false;
	depthstencildesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthstencildesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthstencildesc.StencilEnable = true;
	depthstencildesc.StencilReadMask = 0xFF;
	depthstencildesc.StencilWriteMask = 0xFF;

	depthstencildesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthstencildesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthstencildesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthstencildesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	depthstencildesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthstencildesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthstencildesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthstencildesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	hr = gfx_device->CreateDepthStencilState(&depthstencildesc, &gfx_depthoff);
	if(!gfx_CheckDXError(hr))
		return false;

	D3D11_DEPTH_STENCIL_VIEW_DESC depthviewdesc;
	ZeroMemory(&depthviewdesc, sizeof(depthviewdesc));

	depthviewdesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthviewdesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthviewdesc.Texture2D.MipSlice = 0;

	hr = gfx_device->CreateDepthStencilView(gfx_depthstencilbuffer, &depthviewdesc, &gfx_depthstencilview);
	if(!gfx_CheckDXError(hr))
		return false;
	
	gfx_devicecontext->OMSetRenderTargets(1, &gfx_rendertargetview, gfx_depthstencilview);

	D3D11_RASTERIZER_DESC rasterdesc;
	ZeroMemory(&rasterdesc, sizeof(rasterdesc));
	rasterdesc.AntialiasedLineEnable = false;
	rasterdesc.CullMode = D3D11_CULL_BACK;
	rasterdesc.DepthBias = 0;
	rasterdesc.DepthBiasClamp = 0.0f;
	rasterdesc.DepthClipEnable = true;
	rasterdesc.FillMode = D3D11_FILL_SOLID;
	rasterdesc.FrontCounterClockwise = false;
	rasterdesc.MultisampleEnable = false;
	rasterdesc.ScissorEnable = false;
	rasterdesc.SlopeScaledDepthBias = 0.0f;

	hr = gfx_device->CreateRasterizerState(&rasterdesc, &gfx_renderstate);
	if(!gfx_CheckDXError(hr))
		return false;

	gfx_devicecontext->RSSetState(gfx_renderstate);

	GFX_SetViewport(settings.width, settings.height, 0.0f, 1.0f);
		
	gfx_vsync = settings.vsync;
	return true;
}

/*********************************************************************/

void GFX_Shutdown()
{
	if(gfx_devicecontext)
		gfx_devicecontext->ClearState();

	if(gfx_renderstate) gfx_renderstate->Release();	
	if(gfx_depthstencilview) gfx_depthstencilview->Release();
	if(gfx_depthstencilbuffer) gfx_depthstencilbuffer->Release();
	if(gfx_depthon) gfx_depthon->Release();
	if(gfx_depthoff) gfx_depthoff->Release();

	if(gfx_rendertargetview) gfx_rendertargetview->Release();
	if(gfx_swapchain) gfx_swapchain->Release();
	if(gfx_devicecontext) gfx_devicecontext->Release();
	if(gfx_device) gfx_device->Release();
}

/*********************************************************************/

void GFX_SetViewport(int width, int height, float zmin, float zmax)
{
	D3D11_VIEWPORT vp;
	vp.Width = (float)width;
	vp.Height = (float)height;
	vp.MinDepth = zmin;
	vp.MaxDepth = zmax;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	gfx_devicecontext->RSSetViewports(1, &vp);
}


void GFX_SetClearColour(gfx_colour cc)
{
	gfx_clearcolour = cc;
}

/*********************************************************************/

void GFX_ClearBuffer(int buffers)
{
	if(buffers & GFX_BUFFER_COLOUR)
	{			
		gfx_devicecontext->ClearRenderTargetView(gfx_rendertargetview, (float*)&gfx_clearcolour);		
	}

	if(buffers & GFX_BUFFER_DEPTH)
	{
		gfx_devicecontext->ClearDepthStencilView(gfx_depthstencilview, D3D11_CLEAR_DEPTH, 1.0f, 0);
	}
}

/*********************************************************************/

void GFX_Swap()
{	
	gfx_swapchain->Present(gfx_vsync ? 1 : 0, 0);
}

/*********************************************************************/

gfx_pixelshader * GFX_LoadPixelShader(const char * filename, const char * entrypoint)
{

	DWORD shaderflags = D3DCOMPILE_ENABLE_STRICTNESS;

#ifdef _DEBUG
	shaderflags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob * shaderblob = 0;
	ID3DBlob * errorblob = 0;
	
	if(FAILED(D3DX11CompileFromFile(filename, 0, 0, entrypoint, "ps_4_0", shaderflags, 0, 0, &shaderblob, &errorblob, 0)))
	{
		if(errorblob)
		{
			//debug output
			errorblob->Release();
		}
		return 0;
	}

	gfx_pixelshader * shader = (gfx_pixelshader*)malloc(sizeof(gfx_pixelshader));

	if(FAILED(gfx_device->CreatePixelShader(shaderblob->GetBufferPointer(), shaderblob->GetBufferSize(), 0, &shader->shader)))
	{
		shader->shader->Release();
		free(shader);
		return 0;
	}

	return shader;
}

/*********************************************************************/

gfx_texture * GFX_LoadTexture(const char * filename)
{
	gfx_texture * t = (gfx_texture*)malloc(sizeof(gfx_texture));	
	D3DX11CreateShaderResourceViewFromFile(gfx_device, filename, 0, 0, &t->dxTexture, 0);
	return t;
}

void GFX_FreeTexture(gfx_texture * texture)
{

}

/*********************************************************************/

static bool gfx_CreateVertexLayout(gfx_vertexshader * shader, ID3DBlob * shaderblob,  gfx_vertexdesc * desc)
{	

	unsigned int nelements = sizeof(*desc) / sizeof(desc[0]);	
	
	D3D11_INPUT_ELEMENT_DESC * d3ddesc = (D3D11_INPUT_ELEMENT_DESC*)malloc(sizeof(D3D11_INPUT_ELEMENT_DESC) * nelements);
	
	for(unsigned int i = 0; i < nelements; ++i)
	{		
		d3ddesc->SemanticName = desc[i].semantic;
		d3ddesc[i].SemanticIndex = i;
		switch(desc->vertexformat)		
		{
		case GFX_VERTEXFORMAT_RG_FLOAT: d3ddesc[i].Format = DXGI_FORMAT_R32G32_FLOAT; break;
		case GFX_VERTEXFORMAT_RG_UINT: d3ddesc[i].Format = DXGI_FORMAT_R32G32_UINT; break;
		case GFX_VERTEXFORMAT_RGB_FLOAT: d3ddesc[i].Format = DXGI_FORMAT_R32G32B32_FLOAT; break;
		case GFX_VERTEXFORMAT_RGB_UINT: d3ddesc[i].Format = DXGI_FORMAT_R32G32B32_UINT; break;
		case GFX_VERTEXFORMAT_RGBA_FLOAT: d3ddesc[i].Format = DXGI_FORMAT_R32G32B32A32_FLOAT; break;
		case GFX_VERTEXFORMAT_RGBA_UINT: d3ddesc[i].Format = DXGI_FORMAT_R32G32B32A32_UINT; break;
			
		default: return false;
		}

		d3ddesc[i].AlignedByteOffset = 0;
		d3ddesc[i].InputSlot = 0;
		d3ddesc[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;		
		d3ddesc[i].InstanceDataStepRate = 0;
	}

	if(FAILED(gfx_device->CreateInputLayout(d3ddesc, nelements, shaderblob->GetBufferPointer(), shaderblob->GetBufferSize(), &shader->layout)))
	{
		return false;
	}
	return true;
}

/*********************************************************************/

gfx_vertexshader * GFX_LoadVertexShader(const char * filename, const char * entrypoint, gfx_vertexdesc * desc)
{

	DWORD shaderflags = D3DCOMPILE_ENABLE_STRICTNESS;

#ifdef _DEBUG
	shaderflags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob * shaderblob = 0;
	ID3DBlob * errorblob = 0;
	
	if(FAILED(D3DX11CompileFromFile(filename, 0, 0, entrypoint, "vs_4_0", shaderflags, 0, 0, &shaderblob, &errorblob, 0)))
	{
		if(errorblob)
		{
			//debug output
			errorblob->Release();
		}
		return 0;
	}

	gfx_vertexshader * shader = (gfx_vertexshader*)malloc(sizeof(gfx_vertexshader));

	if(FAILED(gfx_device->CreateVertexShader(shaderblob->GetBufferPointer(), shaderblob->GetBufferSize(), 0, &shader->shader)))
	{
		shader->shader->Release();
		free(shader);
		return 0;
	}

	if(!gfx_CreateVertexLayout(shader, shaderblob, desc))
	{
		shader->shader->Release();
		free(shader);
		return 0;
	}

	shaderblob->Release();	

	return shader;
}

/*********************************************************************/

gfx_vertexbuffer * GFX_CreateVertexBuffer(void * vertices, unsigned int nvertices, size_t stride, bool staticbuffer)
{
	gfx_vertexbuffer * res = (gfx_vertexbuffer*)malloc(sizeof(gfx_vertexbuffer));

	res->stride = stride;

	D3D11_BUFFER_DESC bufferdesc;
	bufferdesc.Usage            = (staticbuffer) ? D3D11_USAGE_DEFAULT : D3D11_USAGE_DYNAMIC;
	bufferdesc.ByteWidth        = stride * nvertices;
	bufferdesc.BindFlags        = D3D11_BIND_VERTEX_BUFFER;
	bufferdesc.CPUAccessFlags   = (staticbuffer) ? 0 : D3D11_CPU_ACCESS_WRITE;
	bufferdesc.MiscFlags        = 0;

	D3D11_SUBRESOURCE_DATA initdata;
    initdata.pSysMem = vertices;
    initdata.SysMemPitch = 0;
    initdata.SysMemSlicePitch = 0;

	if(FAILED(gfx_device->CreateBuffer(&bufferdesc, &initdata, &res->vbuffer)))
	{
		GFX_FreeVertexBuffer(res);
		return 0;
	}

	return res;
}

void GFX_FreeVertexBuffer(gfx_vertexbuffer * buffer)
{
	buffer->vbuffer->Release();
	buffer->vbuffer = 0;
	free(buffer);
	buffer = 0;
}

void GFX_SetCurrentVertexBuffer(gfx_vertexbuffer * buffer, unsigned int offset)
{
	gfx_devicecontext->IASetVertexBuffers(0, 1, &buffer->vbuffer, &buffer->stride, &offset);
}

void GFX_LockVertexBufferWrite(gfx_vertexbuffer * buffer, void ** data)
{
	D3D11_MAPPED_SUBRESOURCE res;
	
	if(FAILED(gfx_devicecontext->Map(buffer->vbuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &res)))
	{
		*data = 0;
		return;
	}
	*data = (void*)res.pData;
}

void GFX_UnlockVertexBuffer(gfx_vertexbuffer * buffer)
{
	gfx_devicecontext->Unmap(buffer->vbuffer, 0);
}

/*********************************************************************/

void GFX_EnableBuffer(int buffer, bool enable)
{
	switch(buffer)
	{	
	case GFX_BUFFER_DEPTH: gfx_devicecontext->OMSetDepthStencilState(enable ? gfx_depthon : gfx_depthoff, 1);
	default: return;
	}
}

void GFX_SetPrimitiveType(int type)
{
	switch(type)
	{
	case GFX_PRIMITIVETYPE_TRIANGLELIST: gfx_devicecontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); return;
	}
}

void GFX_SetTexture(gfx_texture * tex, int level)
{

	gfx_devicecontext->PSSetShaderResources(level, 1, &tex->dxTexture);
}

/*********************************************************************/

void GFX_DrawPrimitives(gfx_vertexshader * vshader, gfx_pixelshader * pshader, unsigned int nverts)
{
	gfx_devicecontext->VSSetShader(vshader->shader, 0, 0);
	gfx_devicecontext->PSSetShader(pshader->shader, 0, 0);
	gfx_devicecontext->Draw(nverts, 0);
}