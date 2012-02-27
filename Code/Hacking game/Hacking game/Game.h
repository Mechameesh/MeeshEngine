#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED


/************************************************************/
enum game_modetypes
{
	//GAME_MODE_MENU,
	GAME_MODE_HACK,
	GAME_NMODES
};

/************************************************************/

struct game_initsettings
{
	
};

struct game_mode
{
	virtual void Enter() = 0;
	virtual void Leave() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
};

/************************************************************/

bool GAME_Init(game_initsettings settings);
void GAME_ShutDown();
void GAME_Update();
void GAME_Draw();

/************************************************************/

void GAME_PushMode(int mode);
void GAME_PushModeLight(int mode);
void GAME_PopMode();
void GAME_PopModeLight();


/************************************************************/

#endif //GAME_H_INCLUDED