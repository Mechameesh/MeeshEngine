#include "ft2build.h"
#include FT_FREETYPE_H
#include <math.h>
#include <SFML\Graphics.hpp>
#include <SFML\System.hpp>

#ifdef _DEBUG
#pragma comment(lib, "freetype2410_D")
#else
#pragma comment(lib, "freetype2410")
#endif

#ifdef _DEBUG
#pragma comment(lib, "sfml-graphics-s-d")
#else
#pragma comment(lib, "sfml-graphics-s")
#endif

#ifdef _DEBUG
#pragma comment(lib, "sfml-system-s-d")
#else
#pragma comment(lib, "sfml-system-s")
#endif



struct uv
{
	float u0;
	float v0;
	float u1;
	float v1;
};

struct charset_t
{
	char texfile[256];
	int nchars;
	char * chars;
	uv * uvs;
	unsigned char * advances;	
};

static charset_t charset;
static FT_Library library;
static FT_Face face;

static int RoundToPow2(int n, bool roundup)
{
	int p0 = 1;
	int p1 = p0;

	while(n > p0)
	{
		p1 = p0;
		p0 <<= 1;

		//make sure we don't wrap
		if(p0 < p1)
			return -1;
	}

	return roundup ? p0 : p1;
}

static void LoadFont(char * name, int sizex, int sizey, char * outputname)
{
	if(FT_Init_FreeType(&library))
		return;

	if(FT_New_Face(library, name, 0, &face))
		return;

	if(FT_Set_Pixel_Sizes(face, sizex, sizey))
		return;

	//should be cleaned up to take input
	charset.nchars = 0;
	charset.chars = (char*)malloc(sizeof(char) * 93);
	memset(charset.chars, 0, sizeof(char) * 93);
	charset.uvs = (uv*)malloc(sizeof(uv) * 93);
	memset(charset.uvs, 0, sizeof(uv) * 93);
	charset.advances = (unsigned char*)malloc(sizeof(unsigned char) * 93);
	memset(charset.advances, 0, sizeof(unsigned char) * 93);

	int maxwidth = 0;
	int maxheight  = 0;
	int maxbearingy = 0;
	const int charspace = 5;

	for(char i = 33; i <= 125; ++i)
	{
		charset.chars[charset.nchars++] = i;		
		if(FT_Load_Char(face, i, FT_LOAD_RENDER))
		{
			return;
		}

		if((2 * (face->glyph->bitmap.rows << 6) - face->glyph->metrics.horiBearingY) > maxheight)
			maxheight = (2 * (face->glyph->bitmap.rows << 6) - face->glyph->metrics.horiBearingY);
		if(face->glyph->metrics.horiBearingY > maxbearingy)
			maxbearingy = face->glyph->metrics.horiBearingY;
		if((face->glyph->advance.x >> 6) + (face->glyph->metrics.horiBearingX >> 6) > maxwidth)
			maxwidth = (face->glyph->advance.x >> 6) + (face->glyph->metrics.horiBearingX >> 6);
	}
	
	//figure out texture size
	size_t imagesize = (maxwidth + charspace) * ((maxheight >> 6) + charspace) * charset.nchars;
	int imagewidth, imageheight;
	
	int imagesidesize = (int)(sqrt((float)imagesize));
	//in case of a glyph being chopped in half, add a glyph size for w and h
	if(maxwidth > (maxheight >> 6))
		imagesidesize += maxwidth;
	else
		imagesidesize += (maxheight >> 6);

	int pow2size = RoundToPow2(imagesidesize, true);

	//non-square optimal?
	if(((size_t)pow2size * (size_t)pow2size / 2) >= imagesize)
		imageheight = (size_t)(pow2size / 2);
	else
		imageheight = pow2size;
	
	imagewidth = pow2size;	

	const size_t pixelbytes = 2;
	size_t datawidth = imagewidth * pixelbytes;
	size_t datasize = imagewidth * imageheight * pixelbytes;

	unsigned char * imagedata = (unsigned char *)malloc(sizeof(unsigned char) * datasize);

	for(size_t i = 0; i < datasize; i += pixelbytes)
	{
		imagedata[i] = 0x00;		//luminance
		imagedata[i + 1] = 0x00;	//alpha
	}

	int column = 0;
	int row = 0;
	for(int i = 0; i < charset.nchars; ++i)	
	{
		if(FT_Load_Char(face, charset.chars[i], FT_LOAD_RENDER))
		{
			charset.chars[i] = -1;
			continue;
		}

		FT_Int advance = face->glyph->advance.x >> 6;
		unsigned char * buffer = face->glyph->bitmap.buffer;

		if(!buffer)
		{
			printf("Font does not contain char \"%c\"\n", charset.chars[i]);
			continue;
		}

		int bearingx = face->glyph->metrics.horiBearingX >> 6;
		int bearingy = (maxbearingy >> 6) - (face->glyph->metrics.horiBearingY >> 6);		

		for(int j = 0; j < face->glyph->bitmap.rows; ++j)
		{
			size_t glyphrow = j + row + bearingy;
			int xpos = (column + bearingx) * pixelbytes;
			int ypos = glyphrow * datawidth;
			//unsigned char * dest = &imagedata[(glyphrow * datawidth) + (column + bearingx) * pixelbytes];
			unsigned char * dest = &imagedata[ypos + xpos];
			for(int k = 0; k < face->glyph->bitmap.width; ++k)
			{				
				*dest++= *buffer;	//luminance
				if(*buffer != 0)
					*dest++= *buffer;	//alpha
				else
					*dest++;
				*buffer++;
			}
		}

		charset.uvs[i].u0 = (float)column / (float)imagewidth;
		charset.uvs[i].v0 = (float)row / (float)imageheight;
		charset.uvs[i].u1 = (float)(column + (face->glyph->advance.x >> 6)) / (float)imagewidth;
		charset.uvs[i].v1 = (float)(row + (maxheight >> 6)) / (float)imageheight;

		charset.advances[i] = (unsigned char)advance;

		column += (advance + charspace);

		if(imagewidth - 1 < column + advance)
		{
			row += (maxheight >> 6) + charspace;
			column = 0;
		}
	}	

	sf::Image image = sf::Image();
	
	sf::Color color = sf::Color(0, 0, 0);
	image.create(imagewidth, imageheight, color);
	
	for(int i = 0; i < imageheight; ++i)
	{
		for(int j = 0; j < imagewidth; ++j)
		{			
			int index = i * datawidth + j * pixelbytes;
			color.r = color.g = color.b = imagedata[index];
			color.a = imagedata[index + 1];
			image.setPixel(j, i, color);
		}
	}


	char imagefilename[256];
	sprintf_s(imagefilename, "%s.png", outputname);

	image.saveToFile(imagefilename);	

	strcpy_s(charset.texfile, imagefilename);

	FT_Done_Face(face);
	FT_Done_FreeType(library);

	//finally export char description file
	char fontfilename[256];
	sprintf_s(fontfilename, "%s.fnt", outputname);

	FILE * descfile = 0;
	fopen_s(&descfile, fontfilename, "wb");
	
	if(!descfile)
		return;

	size_t uvsize = sizeof(uv);
	uvsize *= charset.nchars;
	
	fwrite(charset.texfile, sizeof(char), 256, descfile);						//texture file name
	fwrite(&charset.nchars, sizeof(int), 1, descfile);							//nchars
	fwrite(charset.chars, sizeof(char), (size_t)charset.nchars, descfile);
	fwrite(charset.uvs, sizeof(uv), (size_t)charset.nchars, descfile);			//uvs
	fwrite(charset.advances, sizeof(unsigned char), (size_t)charset.nchars, descfile);	//advances

	fclose(descfile);
}

int main(int argc, char * argv[])
{
	/*if (argc < 3)
	{
		printf("Usage: fontripper fontname charset.txt outputimage.png outputdesc.fnt\n");
		return 0;
	}*/
	
	LoadFont("C:/Windows/Fonts/Arial.ttf", 20, 20, "arial");

	return 0;
}