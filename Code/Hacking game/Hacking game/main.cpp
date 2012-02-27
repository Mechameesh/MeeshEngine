#include <Windows.h>
#include "game.h"
#include "input.h"
#include "Graphics.h"
#include "Audio.h"
#include "Timer.h"

HINSTANCE hInstance;
HWND hWnd;

extern input_keyboard keyboard;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch(msg)
	{		
	case WM_PAINT:
		{
			hdc = BeginPaint(hwnd, &ps);
			EndPaint(hwnd, &ps);
		}break;
	case WM_KEYDOWN:
		{
			keyboard.keydown[wparam] = true;
		} break;
	case WM_KEYUP:
		{
			keyboard.keydown[wparam] = false;
		} break;
	case WM_QUIT:
		{
			GAME_ShutDown();
			AUDIO_Shutdown();
			GFX_Shutdown();
		} break;
	default:
		return DefWindowProc(hwnd, msg, wparam, lparam);
	}
	return 0;	
}

bool InitWindow()
{
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = "MusicGameWinClass";

	if(!RegisterClass(&wc))
		return false;

	hWnd = CreateWindow("MusicGameWinClass", "Music Game", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, 0, 0, hInstance, 0);

	if(!hWnd)
		return false;

	ShowWindow(hWnd, SW_SHOW);
	return true;
}

static gfx_initsettings SetupGraphicsSettings()
{
	gfx_initsettings s;
	s.width = 800;
	s.height = 600;
	s.clearcolour.r = 0.0f;
	s.clearcolour.g = 0.125f;
	s.clearcolour.b = 0.6f;
	s.clearcolour.a = 1.0f;
	s.fullscreen = false;
	return s;
}

static audio_initsettings SetupAudioSettings()
{
	audio_initsettings s;
	s.frequency = 44100;
	s.bits = 16;
	s.channels = 2;
	s.hwnd = hWnd;
	return s;
}

static game_initsettings SetupGameSettings()
{
	game_initsettings s;
	return s;
}

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	hInstance = hinstance;

	if(!InitWindow())
		return 0;

	gfx_initsettings gfxSettings = SetupGraphicsSettings();
	if(!GFX_Init(gfxSettings))
		return 0;

	audio_initsettings audioSettings = SetupAudioSettings();
	if(!AUDIO_Init(audioSettings))
		return 0;

	game_initsettings settings = SetupGameSettings();
	if(!GAME_Init(settings))
		return 0;

	TIME_Init();

	MSG msg = {0};	
	while(msg.message != WM_QUIT)
	{
		if(PeekMessage(&msg, hWnd, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			GAME_Update();
			GAME_Draw();
		}
	}

	return 0;
}