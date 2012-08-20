#include <Windows.h>
#include <Windowsx.h>
#include "game.h"
#include "input.h"
#include "Graphics.h"
#include "Audio.h"
#include "Timer.h"
#include "Shaders.h"

HINSTANCE hInstance;
HWND hWnd;

extern input_keyboard keyboard;
extern input_mouse mouse;

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
	case WM_CHAR:
		{			
			char input = wparam;

			if(input = '\b')
			{
				if(keyboard.bufferpos > 1)
				{
					keyboard.bufferpos--;
					keyboard.buffer[keyboard.bufferpos] = 0;
				}
			}
			else if(keyboard.filter[input])
			{
				if(keyboard.bufferpos < (INPUT_KEYBOARDBUFFER_SIZE - 1))
				{
					keyboard.buffer[keyboard.bufferpos] = input;
					keyboard.bufferpos++;
					keyboard.buffer[keyboard.bufferpos] = 0;
				}
			}
		} break;
	case WM_LBUTTONDOWN:
		{
			mouse.buttondown[INPUT_MOUSE_LBUTTON] = true;
		} break;
	case WM_LBUTTONUP:
		{
			mouse.buttondown[INPUT_MOUSE_LBUTTON] = false;
		} break;
	case WM_MBUTTONDOWN:
		{
			mouse.buttondown[INPUT_MOUSE_MBUTTON] = true;
		} break;
	case WM_MBUTTONUP:
		{
			mouse.buttondown[INPUT_MOUSE_MBUTTON] = false;
		} break;
	case WM_RBUTTONDOWN:
		{
			mouse.buttondown[INPUT_MOUSE_RBUTTON] = true;
		} break;
	case WM_RBUTTONUP:
		{
			mouse.buttondown[INPUT_MOUSE_RBUTTON] = false;
		} break;
	case WM_MOUSEMOVE:
		{
			int xpos = GET_X_LPARAM(lparam);
			int ypos = GET_Y_LPARAM(lparam);
			if(mouse.locked)
			{	
				int centerx = GFX_GetXRes() / 2;
				int centery = GFX_GetYRes() / 2;
				mouse.deltax = xpos - centerx;
				mouse.deltay = ypos - centery;
				SetCursorPos(centerx, centery);
			}
			else
			{
				mouse.xpos = xpos;
				mouse.ypos = ypos;
			}
		} break;
	case WM_DESTROY:	
		{
			PostQuitMessage(0);
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

	if(!AUDIO_Init())
		return 0;

	SHADER_InitShaders();

	game_initsettings settings = SetupGameSettings();
	if(!GAME_Init(settings))
		return 0;

	TIME_Init();
	INPUT_Init();
	INPUT_ApplyDefaultFilter();

	MSG msg = {0};	
	while(true)
	{
		if(PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			GAME_Update();
			GAME_Draw();
		}
	}

	GAME_ShutDown();
	AUDIO_Shutdown();
	GFX_Shutdown();

	return msg.wParam;
}