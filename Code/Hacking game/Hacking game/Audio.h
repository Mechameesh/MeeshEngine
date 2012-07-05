#ifndef AUDIO_H_INCLUDED
#define AUDIO_H_INCLUDED

/******************************************************/

#include <Windows.h>
#include <MMSystem.h>
#include <XAudio2.h>

enum
{
	AUDIO_CONTEXT_SFX,
	AUDIO_CONTEXT_SFX3D,
	AUDIO_CONTEXT_MUSIC,
	AUDIO_NCONTEXTS
};

enum
{
	AUDIO_STATE_STOPPED,
	AUDIO_STATE_PLAYING,
	AUDIO_STATE_PAUSED
};

struct audio_sound
{
	WAVEFORMATEX wfx;
	void * data;
	XAUDIO2_BUFFER buffer;
	int context;
};

struct audio_context
{	
	IXAudio2SubmixVoice * mixvoice;	
	int nvoices;
};


/******************************************************/

bool AUDIO_Init();
void AUDIO_Shutdown();

void AUDIO_Update();

#endif //AUDIO_H_INCLUDED