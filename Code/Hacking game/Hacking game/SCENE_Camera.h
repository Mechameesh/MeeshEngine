#ifndef SCENE_CAMERA_H_INCLUDED
#define SCENE_CAMERA_H_INCLUDED

#include "SYS_Math.h"

struct scene_camera
{
	bool orthographic;
	float fov;
	math_matrix4 trans;
	math_matrix4 projection;
};

scene_camera * SCENE_CreateCamera(float fov, const math_vec3 * pos, bool orthographic);
void SCENE_GetCameraPos(scene_camera * camera, math_vec3 * pos);
void SCENE_SetCameraPos(scene_camera * camera, const math_vec3 * pos);
void SCENE_GetViewMatrixFromCamera(scene_camera * camera, math_matrix4 * view);
void SCENE_FreeCamera(scene_camera * camera);

#endif //SCENE_CAMERA_H_INCLUDED