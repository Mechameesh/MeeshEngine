#ifndef SCENE_CAMERA_H_INCLUDED
#define SCENE_CAMERA_H_INCLUDED

#include "SYS_Math.h"

struct scene_camera
{
	float fov;
	math_matrix4 position;
	math_matrix4 projection;
};


#endif //SCENE_CAMERA_H_INCLUDED