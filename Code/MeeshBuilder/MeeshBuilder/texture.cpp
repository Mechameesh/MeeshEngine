#include "stdafx.h"
#include "texture.h"

void TEX_ConvertToPNG(const char * filename, char * newfilename)
{
	ILuint imagename = ilGenImage();
	ilBindImage(imagename);

	wchar_t wfilename[256];
	size_t numchars = 0;
	mbstowcs_s(&numchars, wfilename, filename, 256);
	ilLoadImage(wfilename);

	ilEnable(IL_FILE_OVERWRITE);
	strcpy_s(newfilename, 256, filename);
	int length = strlen(newfilename);
	newfilename[length - 3] = 'p';
	newfilename[length - 2] = 'n';
	newfilename[length - 1] = 'g';

	mbstowcs_s(&numchars, wfilename, newfilename, 256);
	ilSave(IL_PNG, wfilename);

	ilDeleteImage(imagename);
}
