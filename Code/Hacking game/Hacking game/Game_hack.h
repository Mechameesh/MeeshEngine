#ifndef GAME_HACK_H_INCLUDED
#define GAME_HACK_H_INCLUDED

/******************************************************/
#include "Game.h"

/******************************************************/

#define HACK_MAXSOUNDBUFFER (44100 * 2)			//frequency  * seconds

/******************************************************/

enum
{
	HACK_SOUNDTYPE_SINE,
	HACK_SOUNDTYPE_SQUARE,
	HACK_SOUNDTYPE_SAWTOOTH,
	HACK_SOUNDTYPE_NOISE,
	HACK_NSOUNDTYPES
};

/******************************************************/
struct audio_buffer;
struct scene_camera;
struct scene_mesh;
struct gfx_texture;

struct game_hackmode : game_mode
{
	void Enter();
	void Leave();
	void Update(int dt);
	void Draw();

	int packetTime;

	unsigned int soundpos;
	short sounddata[HACK_MAXSOUNDBUFFER];
	unsigned int sounddatasize;

	scene_camera * camera;
	scene_mesh * mesh;

	audio_buffer * sound;	

	void GenerateHack();
};

/******************************************************/



game_mode * GAME_CreateHackMode();

/******************************************************/

#endif //GAME_HACK_H_INCLUDED