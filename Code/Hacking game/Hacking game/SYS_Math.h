#ifndef SYS_MATH_H_INCLUDED
#define SYS_MATH_H_INCLUDED

/******************************************************/

#define M_PI (3.141592653589793)
#define M_HALF_PI (1.570796326794897)

struct math_vec2
{
	float x, y;
};

struct math_vec3
{
	float x, y, z;
};

struct math_vec4
{
	float x, y, z, w;
};

struct math_matrix3
{
	float m[3][3];
};

struct math_matrix4
{
	float m[4][4];
};

void MATH_Vector2Add(math_vec2 * res, math_vec2 * a, math_vec2 * b);
void MATH_Vector2Subtract(math_vec2 * res, math_vec2 * a, math_vec2 * b);
void MATH_Vector2Scale(math_vec2 * res, math_vec2 * in, float s);
float MATH_Vector2Dot(math_vec2 * res, math_vec2 * a, math_vec2 * b);
void MATH_Vector2Invert(math_vec2 * res, math_vec2 * in);
float MATH_Vector2Length(math_vec2 * in);

void MATH_Vector3Add(math_vec3 * res, math_vec3 * a, math_vec3 * b);
void MATH_Vector3Subtract(math_vec3 * res, math_vec3 * a, math_vec3 * b);
void MATH_Vector3Scale(math_vec3 * res, math_vec3 * in, float s);
float MATH_Vector3Dot(math_vec3 * a, math_vec3 * b);
void MATH_Vector3Cross(math_vec3 * res, math_vec3 * a, math_vec3 * b);

void MATH_Matrix3Identity(math_matrix3 * in);
void MATH_Matrix3Concat(math_matrix3 * res, math_matrix3 * a, math_matrix3 * b);
void MATH_Matrix3Transpose(math_matrix3 * res, math_matrix3 * in);

void MATH_Matrix4Identity(math_matrix4 * in);
void MATH_Matrix4Concat(math_matrix4 * res, math_matrix4 * a, math_matrix4 * b);
void MATH_Matrix4Transpose(math_matrix4 * res, math_matrix4 * in);

void MATH_Matrix4Ortho(math_matrix4 * in);

/******************************************************/

#endif SYS_MATH_H_INCLUDED