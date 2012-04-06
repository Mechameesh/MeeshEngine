#ifndef SHADERS_H_INCLUDED
#define SHADERS_H_INCLUDED


struct gfx_texture;
struct gfx_colour;
struct math_vec2;
struct math_matrix4;
struct scene_mesh;

enum
{
	SHADER_SPRITESHADER,
	SHADER_STANDARDSHADER,
	SHADER_FLATSHADER,
	NSHADERS
};

void SHADER_InitShaders();

void SHADER_UpdateShaders(const math_matrix4 * wvp);

void SHADER_DrawMesh(scene_mesh * mesh, int shader);

void SHADER_RenderSpriteAbs(int x, int y, int width, int height, const gfx_texture * texture, const gfx_colour * colour);
void SHADER_RenderSpriteAbsUV(int x, int y, int width, int height, float u0, float u1, float v0, float v1, const gfx_texture * texture, const gfx_colour * colour);
void SHADER_RenderSprite(float x, float y, float width, float height, const gfx_texture * texture, const gfx_colour * colour);
void SHADER_RenderSpriteUV(float x, float y, float width, float height, float u0, float v0, float u1, float v1, const gfx_texture * texture, const gfx_colour * colour);


#endif //SHADERS_H_INCLUDED
