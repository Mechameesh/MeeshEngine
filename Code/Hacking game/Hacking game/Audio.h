#ifndef AUDIO_H_INCLUDED
#define AUDIO_H_INCLUDED

#include <Windows.h>



struct IDirectSoundBuffer8;
struct IDirectSound3DBuffer8;

/******************************************************/

struct audio_initsettings
{	
	int frequency;
	int bits;
	int channels;	
	HWND hwnd;
};

struct stream_info
{	
	char * source;
	unsigned int sourcesize;
	unsigned int sourcepos;
};

struct audio_buffer
{
	IDirectSoundBuffer8 * buffer;
	IDirectSound3DBuffer8 * buffer3D;	
	unsigned int size;
	bool looping;	
	bool isstream;
	stream_info stream;
};

/******************************************************/

bool AUDIO_Init(audio_initsettings);
void AUDIO_Shutdown();

audio_buffer * AUDIO_LoadFile(const char * filename);

audio_buffer * AUDIO_CreateStreamBuffer(bool is3d, bool looping, char * source, unsigned int sourcelength);
void AUDIO_FreeBuffer(audio_buffer * b);

void AUDIO_SetVolume(audio_buffer * b, float volume);
void AUDIO_SetLooping(audio_buffer * b, bool looping);
void AUDIO_Play(audio_buffer * b);
void AUDIO_Pause(audio_buffer * b);
void AUDIO_Stop(audio_buffer * b);

int AUDIO_RegisterStream(audio_buffer * b);
int AUDIO_UnRegisterStream(audio_buffer * b);

/******************************************************/

void AUDIO_Update(unsigned int dt);

/******************************************************/

#endif AUDIO_H_INCLUDED