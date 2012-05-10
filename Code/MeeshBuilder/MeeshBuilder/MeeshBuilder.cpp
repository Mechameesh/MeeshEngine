// MeeshBuilder.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "mesh.h"

int main(int argc, const char * argv[])
{
	if(argc == 2)
		MESH_ConvertMesh(argv[1], 0);
	else if(argc == 3)
		MESH_ConvertMesh(argv[1], argv[2]);
	else
		printf("Need a .3ds file as input");
	
	return 0;
}

