#include <stdio.h>
#include "GameGlobal.h"
#include "Graphics.h"
#include "Game.h"
#include "Input.h"


LRESULT WINAPI WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_DESTROY:
		//release the Direct3D device
		if (GameGlobal::d3ddev != NULL) {
			GameGlobal::d3ddev->Release();
		}
		//release the Direct3D object
		if (GameGlobal::d3d != NULL) {
			GameGlobal::d3d->Release();
		}
		//shutdown function
		Game::Game_End(hWnd);
		//tell Windows to kill this program
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

//ham ho tro khoi dong Window Properties
ATOM MyRegisterClass(HINSTANCE hInstance) {
	//create window class structure
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	//Dien tham so ham vao struct
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "Megaman_X3";
	wc.hIconSm = NULL;
	//Dang ky lop cua so
	return RegisterClassEx(&wc);
}

//Dau vao ung dung Window
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	//Dang ky lop cua so
	MyRegisterClass(hInstance);

	//Khoi dong ung dung
	//Luu y: ta da bo qua loi goi InitApplication
	HWND hWnd;
	//Tao 1 cua so
	hWnd = CreateWindow(
		"Megaman_X3",	//window class
		"Megaman_X3",	//title bar
		WS_OVERLAPPEDWINDOW,	//window style
		CW_USEDEFAULT,	//x position of window
		CW_USEDEFAULT,	//y position of window
		GameGlobal::wndWidth,	//width of the window
		GameGlobal::wndHeight,	//height of the window
		NULL,	//parent window
		NULL,	//menu
		hInstance,	//application instance
		NULL	//window parameters
	);

	//Kiem tra loi neu ko tao duoc cua so
	if (!hWnd) {
		return FALSE;
	}

	//Hien thi cua so
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	if (!Graphics::Init_Direct3D(hWnd, GameGlobal::wndWidth, GameGlobal::wndHeight)) {
		MessageBox(hWnd, "Error initializing Direct3D", "Error", MB_OK);
		return 0;
	}

	if (!Sound::Init_DirectSound(hWnd)) {
		MessageBox(hWnd, "Error initializing DirectSound", "Error", MB_OK);
		return 0;
	}

	if (!Input::Init_DirectInput(hWnd)) {
		MessageBox(hWnd, "Error initializing DirectInput", "Error", MB_OK);
		return 0;
	}

	//Khoi tao game
	if (Game::Game_Init(hWnd) == 0) {
		MessageBox(hWnd, "Error initializing the game", "Error", MB_OK);
		return 0;
	}


	//Vong lap thong diep chinh
	int done = 0;
	while (!done) {
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
			//Kiem tra dieu kien thoat
			if (msg.message == WM_QUIT) {
				done = 1;
			}
			//Giai ma thong diep va chuyen lai cho WinProc
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			//xu ly game
			Game::Game_Run(hWnd);
		}
	}

	return msg.wParam;
}