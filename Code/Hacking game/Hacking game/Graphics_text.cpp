#include <stdio.h>
#include <cstdarg>
#include "SYS_File.h"
#include "Graphics.h"
#include "SYS_Math.h"
#include "Shaders.h"

/*********************************************************************/

static char gfx_textbuffer[512];
static math_vec3 gfx_textpos = {0.0f, 0.0f, 0.5f};
static math_matrix4 gfx_textorientation;
static float gfx_lineheight = 0;
static float gfx_characterspace = 0;
static gfx_font * gfx_currentfont = 0;
static math_vec2 gfx_textsize = {1.0f, 1.0f};
static gfx_colour gfx_textcolour = {1.0f, 1.0f, 1.0f, 1.0f};

/*********************************************************************/

gfx_font * GFX_LoadFont(const char * filename)
{
	FILE * fp = 0;
	fopen_s(&fp, filename, "rb");

	if(!fp)
		return 0;

	gfx_font * font = (gfx_font*)malloc(sizeof(gfx_font));
	memset(font, 0, sizeof(gfx_font));
	
	char texturefilename[256];

	for(int i = 0; i < 256; ++i)
		SYS_FileReadChar(fp, texturefilename + i);

	SYS_FileReadInt(fp, &font->ncharacters);

	font->characters = (char*)malloc(sizeof(char) * font->ncharacters);
	font->uvs = (gfx_uv*)malloc(sizeof(gfx_uv) * font->ncharacters);
	font->advances = (unsigned char*)malloc(sizeof(unsigned char) * font->ncharacters);

	fread(font->characters, sizeof(char), (size_t)font->ncharacters, fp);
	fread(font->uvs, sizeof(gfx_uv), (size_t)font->ncharacters, fp);
	fread(font->advances, sizeof(unsigned char), (size_t)font->ncharacters, fp);

	for(int i = 0; i < font->ncharacters; ++i)
		SYS_FileReadChar(fp, &font->characters[i]);

	for(int i = 0; i < font->ncharacters; ++i)
	{
		SYS_FileReadFloat(fp, &font->uvs[i].u0);
		SYS_FileReadFloat(fp, &font->uvs[i].v0);
		SYS_FileReadFloat(fp, &font->uvs[i].u1);
		SYS_FileReadFloat(fp, &font->uvs[i].v1);
	}

	for(int i = 0; i < font->ncharacters; ++i)
		SYS_FileReadUChar(fp, &font->advances[i]);

	fclose(fp);

	char fulltexturefile[256] = "data/textures/";

	strcat_s(fulltexturefile, texturefilename);

	font->texture = GFX_LoadTexture(fulltexturefile);

	return font;
}

/*********************************************************************/

void GFX_FreeFont(gfx_font * font)
{
	if(font)
	{
		GFX_FreeTexture(font->texture);
		if(font->characters)
		{
			free(font->characters);
			font->characters = 0;
		}
		if(font->uvs)
		{
			free(font->uvs);
			font->uvs = 0;
		}
		if(font->advances)
		{
			free(font->advances);
			font->advances = 0;
		}

		free(font);
		font = 0;
	}
}

/*********************************************************************/

void GFX_SetFont(gfx_font * font)
{
	gfx_currentfont = font;
}

/*********************************************************************/

void GFX_SetTextPos2D(float x, float y)
{
	gfx_textpos.x = x;
	gfx_textpos.y = y;
}

void GFX_SetTextPos2Dabs(int x, int y)
{	
	gfx_textpos.x = (float)((GFX_GetXRes() / 2) * -1) + (float)x;
	gfx_textpos.y = (float)(GFX_GetYRes() / 2) - (float)y;
}

void GFX_SetTextPos3D(const math_matrix4 * world)
{
	gfx_textorientation = *world;	
}

void GFX_SetLineHeight(int height)
{
	gfx_lineheight = (float)height / (float)GFX_GetYRes();
}

void GFX_SetCharacterSpace(int space)
{
	gfx_characterspace = (float)space / (float)GFX_GetXRes();
}

void GFX_SetTextSize(float x, float y)
{
	gfx_textsize.x = x;
	gfx_textsize.y = y;
}

void GFX_SetTextColour(gfx_colour * colour)
{
	gfx_textcolour = *colour;
}

/*********************************************************************/

void GFX_PrintText2D(char * string, ...)
{	
	if(!gfx_currentfont)
		return;

	va_list args;
	int textlength;

	va_start(args, string);
	textlength = _vsnprintf_s(gfx_textbuffer, sizeof(gfx_textbuffer), _TRUNCATE, string, args);
	va_end(args);	

	for(int i = 0; i < textlength; ++i)
	{
		char c = gfx_textbuffer[i];
		if(c == '\n')
		{
			gfx_textpos.y += gfx_lineheight;			
			continue;
		}

		int charindex = c - 33;

		if(gfx_currentfont->characters[charindex] == -1 || c == 32)
		{
			gfx_textpos.x += gfx_characterspace;
			continue;
		}

		float advance = (float)gfx_currentfont->advances[charindex] / (float)(GFX_GetXRes() * 2) * gfx_textsize.x;
		SHADER_RenderSpriteUV(gfx_textpos.x, gfx_textpos.y, advance, gfx_lineheight * gfx_textsize.y,
			gfx_currentfont->uvs[charindex].u0, gfx_currentfont->uvs[charindex].v0, gfx_currentfont->uvs[charindex].u1, gfx_currentfont->uvs[charindex].v1, gfx_currentfont->texture, &gfx_textcolour);
		gfx_textpos.x += advance;
	}
}

/*********************************************************************/

void GFX_PrintText3D(char * string, ...)
{
}

/*********************************************************************/