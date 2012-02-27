#include "Game_hack.h"
#include "Game.h"
#include "Audio.h"
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include "SYS_Math.h"

/******************************************************/

#define HACK_TIME_DATALENGTH 300		//time in ms for each data byte
#define HACK_TIME_DELTA 200

/******************************************************/

static void hack_GenerateSound(short * data, unsigned int nsamples, unsigned int frequency, int type)
{
	short amplitude = SHRT_MAX;
	double angle = (M_PI * 2 * frequency) / 44100;
			
	switch(type)
	{
	case HACK_SOUNDTYPE_SINE:
		{
			for(unsigned int i = 0; i < nsamples; ++i)
				data[i] = (short)((float)amplitude * sin(angle * i));
		} break;
	case HACK_SOUNDTYPE_SQUARE:
		{
			for(unsigned int i = 0; i < nsamples; ++i)
			{
				if(sin(angle * i) > 0)
					data[i] = amplitude;
				else
					data[i] = -amplitude;
			}
		} break;
	case HACK_SOUNDTYPE_SAWTOOTH:
		{
			unsigned int samplesperperiod = 44100 / frequency;
			short samplestep = (amplitude * 2) / samplesperperiod;
			short tempsample = 0;
			unsigned int i = 0;
			unsigned int written = 0;
			while(written < nsamples)
			{
				tempsample = -amplitude;
				for(i = 0; i < samplesperperiod && written < nsamples; ++i)
				{
					tempsample += samplestep;
					data[written] = tempsample;
					written++;
				}
			}


		} break;
	case HACK_SOUNDTYPE_NOISE:
		{
			for(unsigned int i = 0; i < nsamples; ++i)
			{
				data[i] = SHRT_MIN + (rand() % (SHRT_MAX * 2));
			}
		} break;
	}
}

/******************************************************/

void game_hackmode::GenerateHack()
{
	packetTime = 600 + (rand() % 1400);
	packetTime -= packetTime % HACK_TIME_DATALENGTH;
	int ndatum = (packetTime / HACK_TIME_DATALENGTH);
	unsigned int samplesperdatum = ndatum * 44100 / 1000;

	unsigned int sampleswritten = 0;

	for(int i = 0; i < ndatum; ++i)
	{
		unsigned int frequency = 300 + (rand() % 9700);
		int type = rand() % HACK_NSOUNDTYPES;
		hack_GenerateSound(sounddata + sampleswritten, samplesperdatum, frequency, type);
		sampleswritten += samplesperdatum;
	}
	sounddatasize = sampleswritten * sizeof(short);
}

/******************************************************/

game_mode * GAME_CreateHackMode()
{
	game_hackmode * hm = new game_hackmode;
	return (game_mode*)hm;
}

void game_hackmode::Enter()
{
	GenerateHack();

	sound = AUDIO_CreateStreamBuffer(false, true, (char*)sounddata, sounddatasize);
	AUDIO_RegisterStream(sound);
	AUDIO_SetVolume(sound, 1.0f);
	AUDIO_Play(sound);
}

void game_hackmode::Leave()
{	
	AUDIO_UnRegisterStream(sound);
	AUDIO_FreeBuffer(sound);
}

void game_hackmode::Update()
{
			
}

void game_hackmode::Draw()
{
}

/******************************************************/