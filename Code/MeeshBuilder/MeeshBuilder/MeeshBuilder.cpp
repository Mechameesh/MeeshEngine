// MeeshBuilder.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "mesh.h"

int main(int argc, const char * argv[])
{
	if(argc == 2)
		Convert3DS(argv[1]);
	else
		printf("Need a .3ds file as input");
	
	return 0;
}

