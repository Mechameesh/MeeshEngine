#pragma once

#include "Game.h"

struct game_mistborn : game_mode
{
	void Enter();
	void Leave();
	void Update(int dt);
	void Draw();


};

game_mode * GAME_CreateMistborn();