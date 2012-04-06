#include "SCENE_Camera.h"
#include "Graphics.h"
#include "SYS_Math.h"
#include <string>

scene_camera * SCENE_CreateCamera(float fov, const math_vec3 * pos, bool orthographic)
{
	scene_camera * camera = (scene_camera *)malloc(sizeof(scene_camera));
	
	camera->fov = fov;
	camera->orthographic = orthographic;

	MATH_Matrix4Identity(&camera->trans);
	camera->trans.m[3][0] = pos->x;
	camera->trans.m[3][1] = pos->y;
	camera->trans.m[3][2] = pos->z;
	
	if(camera->orthographic)
		MATH_Matrix4Ortho(&camera->projection, GFX_GetXRes(), GFX_GetYRes(), 1.0f, 1000.0f);
	else
		MATH_Matrix4Projection(&camera->projection, GFX_GetXRes(), GFX_GetYRes(), camera->fov, 1.0f, 1000.f); 

	
	return camera;
}

void SCENE_GetCameraPos(scene_camera * camera, math_vec3 * pos)
{
	pos->x = camera->trans.m[3][0];
	pos->y = camera->trans.m[3][1];
	pos->z = camera->trans.m[3][2];	
}

void SCENE_SetCameraPos(scene_camera * camera, const math_vec3 * pos)
{
	camera->trans.m[3][0] = pos->x;
	camera->trans.m[3][1] = pos->y;
	camera->trans.m[3][2] = pos->z;
}

void SCENE_GetViewMatrixFromCamera(scene_camera * camera, math_matrix4 * view)
{
	MATH_Matrix4Copy(view, &camera->trans);		
}

void SCENE_FreeCamera(scene_camera * camera)
{
	if(camera)
	{
		free(camera);
		camera = 0;
	}
}
