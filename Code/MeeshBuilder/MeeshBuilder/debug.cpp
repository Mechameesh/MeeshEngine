#include "stdafx.h"
#include "debug.h"



/******************************************************/

void DEBUG_Output(const char * format, ...)
{
	char buffer[256];	
	va_list args;
	int n;

	va_start(args, format);		
	n = _vsnprintf(buffer, sizeof(buffer - 1), format, args);
	va_end(args);	

	if(n > 0)
	{
		buffer[n] = '\0';
		OutputDebugStringA(buffer);
	}
}

/******************************************************/