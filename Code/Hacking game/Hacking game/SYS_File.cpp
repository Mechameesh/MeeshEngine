#include <string.h>

bool SYS_IsFileExtension(const char * filename, const char * extension)
{
	int length = strlen(filename);
	int extlength = strlen(extension);

	return (strcmp(&filename[length - extlength], extension) == 0);	
}