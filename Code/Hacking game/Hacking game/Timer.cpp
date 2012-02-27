#include <Windows.h>
#include "Timer.h"

static int time_start;
static int time_prev;

static bool time_running = false;

void TIME_Init()
{
	timeBeginPeriod(1);
}

int TIME_GetTime()
{
	return timeGetTime();
}

void TIME_Start()
{
	time_start = timeGetTime();
	time_prev = time_start;
	time_running = true;
}

void TIME_Stop()
{	
	time_running = false;
}

int TIME_GetDelta()
{
	int now = timeGetTime();
	int delta = now - time_prev;
	time_prev = now;
	return delta;
}

int TIME_GetElapsed()
{
	if(time_running)
		return timeGetTime() - time_start;
	return 0;
}