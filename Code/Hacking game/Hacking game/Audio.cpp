#include <XAudio2.h>
#include <DxErr.h>
#include "Audio.h"
#include "AudioIO.h"
#include "SYS_File.h"

/******************************************************/

/******************************************************/

static IXAudio2 * audio_device = 0;

static IXAudio2MasteringVoice * audio_masterVoice = 0;
static IXAudio2SubmixVoice * audio_mastermix = 0;
static audio_context audio_contexts[AUDIO_NCONTEXTS];

/******************************************************/

static void audio_CreateAudioGraph()
{	
	audio_device->CreateSubmixVoice(&audio_contexts[AUDIO_CONTEXT_SFX].mixvoice,
		1,			//channels
		44100,		//samplerate
		0,			//flags
		0,			//priority
		0,			//destination -> default mastering voice
		0);			//effects -> none for now

	audio_device->CreateSubmixVoice(&audio_contexts[AUDIO_CONTEXT_SFX3D].mixvoice,
		2,			//channels
		44100,		//samplerate
		0,			//flags
		0,			//priority
		0,			//destination -> default mastering voice
		0);			//effects -> none for now

	audio_device->CreateSubmixVoice(&audio_contexts[AUDIO_CONTEXT_MUSIC].mixvoice,
		2,			//channels
		44100,		//samplerate
		0,			//flags
		0,			//priority
		0,			//destination -> default mastering voice
		0);			//effects -> none for now

}

/******************************************************/

bool AUDIO_Init()
{

	CoInitializeEx(0, COINIT_MULTITHREADED);	//COM

	if(FAILED(XAudio2Create(&audio_device, 0, XAUDIO2_DEFAULT_PROCESSOR)))
	{
		return false;
	}

	if(FAILED(audio_device->CreateMasteringVoice(&audio_masterVoice, XAUDIO2_DEFAULT_CHANNELS, XAUDIO2_DEFAULT_SAMPLERATE, 0, 0, 0)))
	{
		audio_device->Release();
		return false;
	}

	audio_CreateAudioGraph();

	return true;
}

/******************************************************/

void AUDIO_Shutdown()
{	
	if(audio_device)
	{
		audio_device->Release();
		audio_device = 0;
	}

	CoUninitialize();	//COM 
}

/******************************************************/

void AUDIO_Update()
{

}

/******************************************************/

audio_sound * AUDIO_CreateSound(const char * filename, int context)
{
	audio_sound * s = 0;
	if(SYS_IsFileExtension(filename, ".wav"))
	{
		s = AUDIO_LoadWaveFile(filename);
		
		s->context = context;
		s->buffer.AudioBytes = s->wfx.cbSize;
		s->buffer.pAudioData = (BYTE *)s->data;
		s->buffer.Flags = XAUDIO2_END_OF_STREAM;
	}
	
	return s;
}

void AUDIO_PlaySound(audio_sound * sound, bool looping)
{
	if(looping)
		sound->buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
	else
		sound->buffer.LoopCount = 0;
}