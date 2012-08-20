#ifndef GAME_EDITOR_H_INCLUDED
#define GAME_EDITOR_H_INCLUDED

#include <vector>
#include "Game.h"
#include "Scene.h"

/******************************************************/
#include "Game.h"

/******************************************************/
/*
struct scene_mesh;

struct edit_scene
{
	vector<scene_mesh> meshes;	
};*/

/******************************************************/

struct scene_camera;
struct gfx_font;
struct gfx_texture;

struct game_editor : game_mode
{
	scene_camera * camera;
	gfx_texture * tex;

	gfx_font * font;

	void Enter();
	void Leave();
	void Update(int dt);
	void Draw();
};

/******************************************************/

game_mode * GAME_CreateEditor();

/******************************************************/

bool EDIT_ExportScene(const char * filename);

/******************************************************/

#endif //GAME_EDITOR_H_INCLUDED