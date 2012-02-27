#include <Windows.h>
#include <dsound.h>
#include <DxErr.h>
#include "Audio.h"

/******************************************************/

#define AUDIO_MAX_STREAMS 64

#define AUDIO_MAX_BUFFER_SIZE (65536)
#define AUDIO_MAX_STREAM_BUFFER_SIZE (AUDIO_MAX_BUFFER_SIZE * 2)

/******************************************************/

static audio_initsettings audio_settings;
static IDirectSound8 * audio_device;
static IDirectSoundBuffer * audio_mainbuffer;
static IDirectSound3DListener8 * audio_listener;

static audio_buffer * audio_activestreams[AUDIO_MAX_STREAMS];

/******************************************************/

struct WaveHeaderType
{
	char chunkId[4];
	unsigned long chunkSize;
	char format[4];
	char subChunkId[4];
	unsigned long subChunkSize;
	unsigned short audioFormat;
	unsigned short numChannels;
	unsigned long sampleRate;
	unsigned long bytesPerSecond;
	unsigned short blockAlign;
	unsigned short bitsPerSample;
	char dataChunkId[4];
	unsigned long dataSize;
};

/******************************************************/

bool AUDIO_Init(audio_initsettings settings)
{
	memcpy(&audio_settings, &settings, sizeof(audio_initsettings));
	HRESULT hr = 0;
	hr = DirectSoundCreate8(0, &audio_device, 0);
	if(FAILED(hr))
	{		
		return false;
	}

	hr = audio_device->SetCooperativeLevel(settings.hwnd, DSSCL_PRIORITY);
	if(FAILED(hr))
	{
		return false;
	}

	DSBUFFERDESC bufdesc;
	bufdesc.dwSize = sizeof(DSBUFFERDESC);
	bufdesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRL3D;
	bufdesc.dwBufferBytes = 0;
	bufdesc.dwReserved = 0;
	bufdesc.lpwfxFormat = 0;
	bufdesc.guid3DAlgorithm = DS3DALG_DEFAULT;

	hr = audio_device->CreateSoundBuffer(&bufdesc, &audio_mainbuffer, 0);
	if(FAILED(hr))
		return false;

	WAVEFORMATEX wf;
	wf.wFormatTag = WAVE_FORMAT_PCM;
	wf.nSamplesPerSec = settings.frequency;
	wf.wBitsPerSample = settings.bits;
	wf.nChannels = settings.channels;
	wf.nBlockAlign = (settings.bits / 8) * settings.channels;
	wf.nAvgBytesPerSec = settings.frequency * wf.nBlockAlign;
	wf.cbSize = 0;

	hr = audio_mainbuffer->SetFormat(&wf);
	if(FAILED(hr))
		return false;

	hr = audio_mainbuffer->QueryInterface(IID_IDirectSound3DListener8, (LPVOID *)&audio_listener);
	if(FAILED(hr))
		return false;

	audio_listener->SetPosition(0.0f, 0.0f, 0.0f, DS3D_IMMEDIATE);

	for(int i = 0; i < AUDIO_MAX_STREAMS; ++i)
		audio_activestreams[i] = 0;
	
	return true;
}

/******************************************************/

void AUDIO_Shutdown()
{
	if(audio_listener)
	{
		audio_listener->Release();
		audio_listener = 0;
	}

	if(audio_mainbuffer)
	{
		audio_mainbuffer->Release();
		audio_mainbuffer = 0;
	}

	if(audio_device)
	{
		audio_device->Release();
		audio_device = 0;
	}
}

/******************************************************/

audio_buffer * audio_CreateBuffer(unsigned int buffersize, bool is3d, bool looping)
{
	HRESULT hr;
	IDirectSoundBuffer * tmpbuffer;

	WAVEFORMATEX wf;
	wf.wFormatTag = WAVE_FORMAT_PCM;
	wf.nSamplesPerSec = audio_settings.frequency;
	wf.wBitsPerSample = audio_settings.bits;
	wf.nChannels = is3d ? 1 : 2;
	wf.nBlockAlign = (audio_settings.bits / 8) * wf.nChannels;
	wf.nAvgBytesPerSec = wf.nSamplesPerSec * wf.nBlockAlign;
	wf.cbSize = 0;
	
	DSBUFFERDESC bufdesc;
	bufdesc.dwSize = sizeof(DSBUFFERDESC);
	bufdesc.dwFlags = DSBCAPS_CTRLVOLUME | (is3d ? DSBCAPS_CTRL3D : 0);
	bufdesc.dwBufferBytes = buffersize;
	bufdesc.dwReserved = 0;
	bufdesc.lpwfxFormat = &wf;
	bufdesc.guid3DAlgorithm = DS3DALG_DEFAULT;

	hr = audio_device->CreateSoundBuffer(&bufdesc, &tmpbuffer, 0);
	if(FAILED(hr))
		return 0;

	audio_buffer * buf = (audio_buffer*)malloc(sizeof(audio_buffer));

	hr = tmpbuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&buf->buffer);
	if(FAILED(hr))
	{
		free(buf);
		buf = 0;
		return 0;
	}

	tmpbuffer->Release();

	if(is3d)
	{
		hr = buf->buffer->QueryInterface(IID_IDirectSound3DBuffer8, (void**)&buf->buffer3D);
		if(FAILED(hr))
		{
			buf->buffer->Release();
			free(buf);
			return 0;
		}
	}			

	buf->size = bufdesc.dwBufferBytes;

	buf->looping = looping;

	return buf;
}

/******************************************************/

static void audio_FeedStream(audio_buffer * b, unsigned int time)
{
	
	void * bufferdata;
	void * wrapbufferdata;
	DWORD buffersize, wrapbuffersize;

	unsigned int bufferfeedsize = (audio_settings.bits / 8) * ((time * audio_settings.frequency) / 1000);

	unsigned int feedsize = bufferfeedsize;
	unsigned int wrapfeedsize = 0;

	if((b->stream.sourcepos + feedsize) > b->stream.sourcesize)
	{
		feedsize = b->stream.sourcesize - b->stream.sourcepos;
		if(b->looping)					
			wrapfeedsize = bufferfeedsize - feedsize;
	}

	b->buffer->Lock(0, feedsize + wrapfeedsize, &bufferdata, &buffersize, &wrapbufferdata, &wrapbuffersize, DSBLOCK_FROMWRITECURSOR);	//flag ignores first param
	
	unsigned int fed = 0;
	unsigned int wrapfed = 0;
	unsigned int totalfeed = feedsize + wrapfeedsize;

	if(totalfeed > buffersize)
	{
		if(feedsize > buffersize)
		{
			unsigned int overflow = feedsize - buffersize;
			memcpy(bufferdata, b->stream.source + b->stream.sourcepos, buffersize);
			fed = buffersize;
			memcpy(wrapbufferdata, b->stream.source + b->stream.sourcepos + buffersize, overflow);
			memcpy((char *)wrapbufferdata + overflow, b->stream.source, wrapfeedsize);
			wrapfed = overflow + wrapfeedsize;
		}
		else
		{
			memcpy(bufferdata, b->stream.source + b->stream.sourcepos, feedsize);
			unsigned int underflow = buffersize - feedsize;

			if(underflow > wrapbuffersize)
			{
				memcpy((char *)bufferdata + feedsize, b->stream.source, underflow);

			}

			fed = feedsize + underflow;
			memcpy(wrapbufferdata, b->stream.source + underflow, wrapfeedsize - underflow);

		}
	}
	else
	{
		memcpy(bufferdata, b->stream.source + b->stream.sourcepos, feedsize);
		memcpy((char *)bufferdata + feedsize, b->stream.source, wrapfeedsize);
		fed = totalfeed;
	}

	b->buffer->Unlock(&bufferdata, fed, &wrapbufferdata, wrapfed);
}
/******************************************************/

audio_buffer * AUDIO_CreateStreamBuffer(bool is3d, bool looping, char * source, unsigned int sourcesize)
{
	audio_buffer * buf = audio_CreateBuffer(AUDIO_MAX_STREAM_BUFFER_SIZE, is3d, looping);
	buf->isstream = true;
	buf->stream.source = source;
	buf->stream.sourcesize = sourcesize;
	buf->stream.sourcepos = 0;

	//prefeed buffer
	audio_FeedStream(buf, 1000);

	return buf;
}

/******************************************************/

void AUDIO_FreeBuffer(audio_buffer * b)
{
	if(b)
	{
		if(b->buffer)
		{
			b->buffer->Release();
			b->buffer = 0;
		}
		if(b->buffer3D)
		{
			b->buffer3D->Release();
			b->buffer3D = 0;
		}
		free(b);
		b = 0;
	}
}

/******************************************************/

audio_buffer * AUDIO_LoadFile(const char * filename)
{
	int len = strlen(filename);

	//if(strc

	return 0;
}

/******************************************************/

void AUDIO_SetVolume(audio_buffer * b, float volume)
{
	float v = (DSBVOLUME_MAX - DSBVOLUME_MIN) * volume;

	b->buffer->SetVolume((int)v);
}

/******************************************************/

void AUDIO_Play(audio_buffer * b)
{
	b->buffer->Play(0, 0, 0);
}

/******************************************************/

void AUDIO_Pause(audio_buffer * b)
{
	b->buffer->Stop();
}

/******************************************************/

void AUDIO_Stop(audio_buffer * b)
{
	b->buffer->Stop();
	b->buffer->SetCurrentPosition(0);
}

int AUDIO_RegisterStream(audio_buffer * b)
{
	for(int i = 0; i < AUDIO_MAX_STREAMS; ++i)
	{
		if(!audio_activestreams[i])
		{
			audio_activestreams[i] = b;
			return i;
		}
	}
	return -1;
}

int AUDIO_UnRegisterStream(audio_buffer * b)
{
	b->buffer->Stop();

	for(int i = 0; i < AUDIO_MAX_STREAMS; ++i)
	{
		if(audio_activestreams[i] == b)
		{
			audio_activestreams[i] = 0;
			return i;
		}
	}
	return -1;
}

/******************************************************/

void AUDIO_Update(unsigned int dt)
{
	//update streams	
	for(int i = 0; i < AUDIO_MAX_STREAMS; ++i)
	{
		if(audio_activestreams[i])
			audio_FeedStream(audio_activestreams[i], dt);
	}
}
/******************************************************/
