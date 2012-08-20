#include "GameEditor.h"
#include "Debug.h"
#include "Input.h"
#include "Graphics.h"
#include "Scene.h"
#include "SCENE_Camera.h"
#include "Shaders.h"

/***************************************************/

void game_editor::Enter()
{
	font = GFX_LoadFont("data/textures/arial.fnt");	

	math_vec3 campos = {0.0f, 0.0f, -10.0f};	
	camera = SCENE_CreateCamera(M_PI / 2.0f, &campos, true);
	tex = GFX_LoadTexture("data/textures/kingdede.png");
}


void game_editor::Leave()
{
	
}

/***************************************************/

void game_editor::Update(int dt)
{	
	math_matrix4 view, wvp;
	SCENE_GetViewMatrixFromCamera(camera, &view);
	MATH_Matrix4Concat(&wvp, &view, &camera->projection);
	MATH_Matrix4Transpose(&wvp, &wvp);
	SHADER_UpdateShaders(&wvp);
}

void game_editor::Draw()
{
	GFX_SetLineHeight(20);
	GFX_SetCharacterSpace(3);
	GFX_SetTextPos2D(0.5f, 0.5f);
	GFX_SetFont(font);
	GFX_SetTextSize(0.5f, 0.5f);
	gfx_colour colour = {1.0f, 1.0f, 1.0f, 1.0f};
	GFX_SetTextColour(&colour);
	GFX_PrintText2D("This is merely a test %d %s", 45, "hah");	

	//SHADER_RenderSprite(0.0f, 0.5f, 0.5f, 0.5f, tex, &colour);
	SHADER_RenderSprite(0.5f, 0.5f, 0.5f, 0.5f, tex, &colour);
	//SHADER_RenderSpriteAbs(0, 300, 400, 300, tex, &colour);
}

/***************************************************/

game_mode * GAME_CreateEditor()
{
	game_editor * edit = new game_editor;
	return (game_mode*)edit;
}

/***************************************************/

bool EDIT_ExportScene(const char * filename)
{
	return false;
}

/***************************************************/