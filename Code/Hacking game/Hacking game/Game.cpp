#include <Windows.h>
#include "Game.h"
#include "Graphics.h"
#include "Input.h"
#include "Audio.h"
#include "Game_hack.h"
#include "Timer.h"

/***************************************************/

#define GAME_MODE_STACKSIZE 8

/***************************************************/

static int game_modestackcurrent = -1;
static int game_modestack[GAME_MODE_STACKSIZE];
static game_mode * game_modes[GAME_NMODES];

/***************************************************/

bool GAME_Init(game_initsettings settings)
{
	memset(game_modestack, -1, sizeof(game_modestack));

	game_modes[GAME_MODE_HACK] = GAME_CreateHackMode();
	
	GAME_PushMode(GAME_MODE_HACK);
	TIME_Start();

	return true;
}

/***************************************************/

void GAME_ShutDown()
{
	TIME_Stop();
	for(int i = 0; i < GAME_NMODES; ++i)
	{
		delete game_modes[i];
	}	
}

/***************************************************/

void GAME_Update()
{
	if(INPUT_KeyDown(VK_ESCAPE))
		PostQuitMessage(0);	

	AUDIO_Update(TIME_GetDelta());
	
	game_modes[game_modestack[game_modestackcurrent]]->Update();	
	
	INPUT_Update();	
}

/***************************************************/

void GAME_Draw()
{
	GFX_ClearBuffer(GFX_BUFFER_COLOUR | GFX_BUFFER_DEPTH);
	
	game_modes[game_modestack[game_modestackcurrent]]->Draw();
	
	GFX_Swap();
}

/***************************************************/

void GAME_PushMode(int mode)
{
	if(game_modestackcurrent == GAME_MODE_STACKSIZE - 1)
		return;

	if(game_modestackcurrent > -1)
		game_modes[game_modestack[game_modestackcurrent]]->Leave();
	
	game_modestackcurrent++;
	game_modestack[game_modestackcurrent] = mode;
	game_modes[game_modestack[game_modestackcurrent]]->Enter();		
}

/***************************************************/

void GAME_PushModeLight(int mode)
{
	if(game_modestackcurrent >= GAME_MODE_STACKSIZE - 1)
		return;

	if(game_modestackcurrent > -1)
		game_modes[game_modestack[game_modestackcurrent]]->Leave();
	
	game_modestackcurrent++;
	game_modestack[game_modestackcurrent] = mode;
	game_modes[mode]->Enter();	
}

/***************************************************/

void GAME_PopMode()
{
	if(game_modestackcurrent > -1)
	{
		game_modes[game_modestack[game_modestackcurrent]]->Leave();
		game_modestackcurrent--;
	}
}

/***************************************************/

void GAME_PopModeLight()
{
	if(game_modestackcurrent > -1)	
		game_modestackcurrent--;
}

/***************************************************/