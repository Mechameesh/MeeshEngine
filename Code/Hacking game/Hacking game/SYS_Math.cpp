#include "SYS_Math.h"
#include <math.h>

/******************************************************************/

void MATH_Vector2Add(math_vec2 * res, math_vec2 * a, math_vec2 * b)
{
	float x = a->x + b->x;
	float y = a->y + b->y;
	res->x = x;
	res->y = y;
}

void MATH_Vector2Subtract(math_vec2 * res, math_vec2 * a, math_vec2 * b)
{
	float x = a->x - b->x;
	float y = a->y - b->y;
	res->x = x;
	res->y = y;
}

void MATH_Vector2Scale(math_vec2 * res, math_vec2 * in, float s)
{
	float x = in->x * s;
	float y = in->y * s;
	res->x = x;
	res->y = y;
}

float MATH_Vector2Dot(math_vec2 * a, math_vec2 * b)
{
	return (a->x * b->x) + (a->y * b->y);
}

void MATH_Vector2Invert(math_vec2 * res, math_vec2 * in)
{
	float x = -in->x;
	float y = -in->y;
	res->x = x;
	res->y = y;
}

float MATH_Vector2Length(math_vec2 * in)
{
	return sqrtf((in->x * in->x) + (in->y * in->y));
}

/******************************************************************/

void MATH_Vector3Add(math_vec3 * res, math_vec3 * a, math_vec3 * b)
{
	float x = a->x + b->x;
	float y = a->y + b->y;
	float z = a->z + b->z;
	res->x = x;
	res->y = y;
	res->z = z;
}

void MATH_Vector3Subtract(math_vec3 * res, math_vec3 * a, math_vec3 * b)
{
	float x = a->x - b->x;
	float y = a->y - b->y;
	float z = a->z - b->z;
	res->x = x;
	res->y = y;
	res->z = z;
}
void MATH_Vector3Scale(math_vec3 * res, math_vec3 * in, float s)
{
	float x = in->x * s;
	float y = in->y * s;
	float z = in->z * s;
	res->x = x;
	res->y = y;
	res->z = z;
}

float MATH_Vector3Dot(math_vec3 * a, math_vec3 * b)
{
	return (a->x * b->x) + (a->y * b->y) + (a->z * b->z);
}

void MATH_Vector3Cross(math_vec3 * res, math_vec3 * a, math_vec3 * b)
{
	float x = (a->y * b->z) - (b->y * a->z);
	float y = (a->z * b->x) - (b->z * a->x);
	float z = (a->x * b->y) - (b->x * a->y);
	res->x = x;
	res->y = y;
	res->z = z;
}

float MATH_Vector3Length(math_vec3 * in)
{
	return sqrtf((in->x * in->x) + (in->y * in->y) + (in->z * in->z));
}

/******************************************************************/
void MATH_Matrix3Identity(math_matrix3 * in)
{
	in->m[0][0] = 1.0f;
	in->m[0][1] = 0.0f;
	in->m[0][2] = 0.0f;
	in->m[1][0] = 0.0f;
	in->m[1][1] = 1.0f;
	in->m[1][2] = 0.0f;
	in->m[2][0] = 0.0f;
	in->m[2][1] = 0.0f;
	in->m[2][2] = 1.0f;
	
}

void MATH_Matrix3Concat(math_matrix3 * res, math_matrix3 * a, math_matrix3 * b)
{
	float m00 = (a->m[0][0] * b->m[0][0]) + (a->m[0][1] * b->m[1][0]) + (a->m[0][2] * b->m[2][0]);
	float m01 = (a->m[0][0] * b->m[0][1]) + (a->m[0][1] * b->m[1][1]) + (a->m[0][2] * b->m[2][1]);
	float m02 = (a->m[0][0] * b->m[0][2]) + (a->m[0][1] * b->m[1][2]) + (a->m[0][2] * b->m[2][2]);
	float m10 = (a->m[1][0] * b->m[0][0]) + (a->m[1][1] * b->m[1][0]) + (a->m[1][2] * b->m[2][0]);
	float m11 = (a->m[1][0] * b->m[0][1]) + (a->m[1][1] * b->m[1][1]) + (a->m[1][2] * b->m[2][1]);
	float m12 = (a->m[1][0] * b->m[0][2]) + (a->m[1][1] * b->m[1][2]) + (a->m[1][2] * b->m[2][2]);
	float m20 = (a->m[2][0] * b->m[0][0]) + (a->m[2][1] * b->m[1][0]) + (a->m[2][2] * b->m[2][0]);
	float m21 = (a->m[2][0] * b->m[0][1]) + (a->m[2][1] * b->m[1][1]) + (a->m[2][2] * b->m[2][1]);
	float m22 = (a->m[2][0] * b->m[0][2]) + (a->m[2][1] * b->m[1][2]) + (a->m[2][2] * b->m[2][2]);

	res->m[0][0] = m00;
	res->m[0][1] = m01;
	res->m[0][2] = m02;
	res->m[1][0] = m10;
	res->m[1][1] = m11;
	res->m[1][2] = m12;
	res->m[2][0] = m20;
	res->m[2][1] = m21;
	res->m[2][2] = m22;
}

void MATH_Matrix3Transpose(math_matrix3 * res, math_matrix3 * in)
{	
	float m01 = in->m[1][0];
	float m02 = in->m[2][0];	
	float m10 = in->m[0][1];
	float m12 = in->m[2][1];	
	float m20 = in->m[0][2];
	float m21 = in->m[1][2];
	
	res->m[0][0] = in->m[0][0];
	res->m[0][1] = m01;
	res->m[0][2] = m02;
	res->m[1][0] = m10;
	res->m[1][1] = in->m[1][1];
	res->m[1][2] = m12;
	res->m[2][0] = m20;
	res->m[2][1] = m21;
	res->m[2][2] = in->m[1][1];
}

/******************************************************************/

void MATH_Matrix4Identity(math_matrix4 * in)
{
	in->m[0][0] = 1.0f;
	in->m[0][1] = 0.0f;
	in->m[0][2] = 0.0f;
	in->m[0][3] = 0.0f;
	in->m[1][0] = 0.0f;
	in->m[1][1] = 1.0f;
	in->m[1][2] = 0.0f;
	in->m[1][3] = 0.0f;
	in->m[2][0] = 0.0f;
	in->m[2][1] = 0.0f;
	in->m[2][2] = 1.0f;
	in->m[2][3] = 0.0f;
	in->m[3][0] = 0.0f;
	in->m[3][1] = 0.0f;
	in->m[3][2] = 0.0f;
	in->m[3][3] = 1.0f;
}

void MATH_Matrix4Concat(math_matrix4 * res, math_matrix4 * a, math_matrix4 * b)
{
	float m00 = (a->m[0][0] * b->m[0][0]) + (a->m[0][1] * b->m[1][0]) + (a->m[0][2] * b->m[2][0]) + (a->m[0][3] * b->m[3][0]);
	float m01 = (a->m[0][0] * b->m[0][1]) + (a->m[0][1] * b->m[1][1]) + (a->m[0][2] * b->m[2][1]) + (a->m[0][3] * b->m[3][1]);
	float m02 = (a->m[0][0] * b->m[0][2]) + (a->m[0][1] * b->m[1][2]) + (a->m[0][2] * b->m[2][2]) + (a->m[0][3] * b->m[3][2]);
	float m03 = (a->m[0][0] * b->m[0][3]) + (a->m[0][1] * b->m[1][3]) + (a->m[0][2] * b->m[2][3]) + (a->m[0][3] * b->m[3][3]);
	float m10 = (a->m[1][0] * b->m[0][0]) + (a->m[1][1] * b->m[1][0]) + (a->m[1][2] * b->m[2][0]) + (a->m[1][3] * b->m[3][0]);
	float m11 = (a->m[1][0] * b->m[0][1]) + (a->m[1][1] * b->m[1][1]) + (a->m[1][2] * b->m[2][1]) + (a->m[1][3] * b->m[3][1]);
	float m12 = (a->m[1][0] * b->m[0][2]) + (a->m[1][1] * b->m[1][2]) + (a->m[1][2] * b->m[2][2]) + (a->m[1][3] * b->m[3][2]);
	float m13 = (a->m[1][0] * b->m[0][3]) + (a->m[1][1] * b->m[1][3]) + (a->m[1][2] * b->m[2][3]) + (a->m[1][3] * b->m[3][3]);
	float m20 = (a->m[2][0] * b->m[0][0]) + (a->m[2][1] * b->m[1][0]) + (a->m[2][2] * b->m[2][0]) + (a->m[2][3] * b->m[3][0]);
	float m21 = (a->m[2][0] * b->m[0][1]) + (a->m[2][1] * b->m[1][1]) + (a->m[2][2] * b->m[2][1]) + (a->m[2][3] * b->m[3][1]);
	float m22 = (a->m[2][0] * b->m[0][2]) + (a->m[2][1] * b->m[1][2]) + (a->m[2][2] * b->m[2][2]) + (a->m[2][3] * b->m[3][2]);
	float m23 = (a->m[2][0] * b->m[0][3]) + (a->m[2][1] * b->m[1][3]) + (a->m[2][2] * b->m[2][3]) + (a->m[2][3] * b->m[3][3]);	
	float m30 = (a->m[3][0] * b->m[0][0]) + (a->m[3][1] * b->m[1][0]) + (a->m[3][2] * b->m[2][0]) + (a->m[3][3] * b->m[3][0]);
	float m31 = (a->m[3][0] * b->m[0][1]) + (a->m[3][1] * b->m[1][1]) + (a->m[3][2] * b->m[2][1]) + (a->m[3][3] * b->m[3][1]);
	float m32 = (a->m[3][0] * b->m[0][2]) + (a->m[3][1] * b->m[1][2]) + (a->m[3][2] * b->m[2][2]) + (a->m[3][3] * b->m[3][2]);
	float m33 = (a->m[3][0] * b->m[0][3]) + (a->m[3][1] * b->m[1][3]) + (a->m[3][2] * b->m[2][3]) + (a->m[3][3] * b->m[3][3]);

	res->m[0][0] = m00;
	res->m[0][1] = m01;
	res->m[0][2] = m02;
	res->m[0][3] = m03;
	res->m[1][0] = m10;
	res->m[1][1] = m11;
	res->m[1][2] = m12;
	res->m[1][3] = m13;
	res->m[2][0] = m20;
	res->m[2][1] = m21;
	res->m[2][2] = m22;
	res->m[2][3] = m23;
	res->m[3][0] = m30;
	res->m[3][1] = m31;
	res->m[3][2] = m32;
	res->m[3][3] = m33;
}

void MATH_Matrix4Ortho(math_matrix4 * in, float width, float height, float near, float far)
{
	float z = far - near;

	in->m[0][0] = 2.0f / width;
	in->m[0][1] = 0.0f;
	in->m[0][2] = 0.0f;
	in->m[0][3] = 0.0f;
	in->m[1][0] = 0.0f;
	in->m[1][1] = 2.0f / height;
	in->m[1][2] = 0.0f;
	in->m[1][3] = 0.0f;
	in->m[2][0] = 0.0f;
	in->m[2][1] = 0.0f;
	in->m[2][2] = 1.0f / z;
	in->m[2][3] = -near / z;
	in->m[3][0] = 0.0f;
	in->m[3][1] = 0.0f;
	in->m[3][2] = 0.0f;
	in->m[3][3] = 1.0f;	
}

void MATH_Matrix4Transpose(math_matrix4 * res, math_matrix4 * in)
{	
	float m01 = in->m[1][0];
	float m02 = in->m[2][0];
	float m03 = in->m[3][0];
	float m10 = in->m[0][1];
	float m12 = in->m[2][1];
	float m13 = in->m[3][1];
	float m20 = in->m[0][2];
	float m21 = in->m[1][2];
	float m23 = in->m[3][2];
	float m30 = in->m[0][3];
	float m31 = in->m[1][3];
	float m32 = in->m[2][3];

	res->m[0][0] = in->m[0][0];
	res->m[0][1] = m01;
	res->m[0][2] = m02;
	res->m[0][3] = m03;
	res->m[1][0] = m10;
	res->m[1][1] = in->m[1][1];
	res->m[1][2] = m12;
	res->m[1][3] = m13;
	res->m[2][0] = m20;
	res->m[2][1] = m21;
	res->m[2][2] = in->m[1][1];
	res->m[2][3] = m23;
	res->m[3][0] = m30;
	res->m[3][1] = m31;
	res->m[3][2] = m32;
	res->m[3][3] = in->m[3][3];
}