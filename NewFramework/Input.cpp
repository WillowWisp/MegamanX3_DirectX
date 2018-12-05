#include "Input.h"

#define BUTTON_DOWN(obj, button) (obj.rgbButtons[button] & 0x80)

LPDIRECTINPUT8 dinput;
LPDIRECTINPUTDEVICE8 dimouse;
LPDIRECTINPUTDEVICE8 dikeyboard;
DIMOUSESTATE mouse_state;

Input::Input()
{
}


Input::~Input()
{
}

//keyboard input
char keys[256];

int Input::Init_DirectInput(HWND hWnd) {
	//init DirectInput object
	HRESULT result = DirectInput8Create(
		GetModuleHandle(NULL),
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&dinput,
		NULL
	);

	if (result != DI_OK) {
		return 0;
	}

	//init the keyboard
	result = dinput->CreateDevice(GUID_SysKeyboard, &dikeyboard, NULL);
	if (result != DI_OK) {
		return 0;
	}
	//init the mouse
	result = dinput->CreateDevice(GUID_SysMouse, &dimouse, NULL);
	if (result != DI_OK) {
		return 0;
	}

	//return ok
	return 1;
}

#pragma region Keyboard
int Input::InitKeyboard(HWND hWnd) {
	//set the data format for keyboard input
	HRESULT result = dikeyboard->SetDataFormat(&c_dfDIKeyboard);
	if (result != DI_OK) {
		return 0;
	}
	//set the cooperative level
	result = dikeyboard->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	if (result != DI_OK) {
		return 0;
	}

	//acquire the keyboard
	result = dikeyboard->Acquire();
	if (result != DI_OK) {
		return 0;
	}

	//return ok
	return 1;
}

void Input::PollKeyboard() {
	dikeyboard->GetDeviceState(sizeof(keys), (LPVOID)&keys);
}

int Input::KeyDown(int key) {
	return (keys[key] & 0x80);
}

void Input::KillKeyboard() {
	if (dikeyboard != NULL) {
		dikeyboard->Unacquire();
		dikeyboard->Release();
		dikeyboard = NULL;
	}
}
#pragma endregion

#pragma region Mouse
int Input::InitMouse(HWND hWnd) {
	//set data format for mouse input
	HRESULT result = dimouse->SetDataFormat(&c_dfDIMouse);
	if (result != DI_OK) {
		return 0;
	}

	//set the cooperative level
	//this will also hide the mouse pointer
	result = dimouse->SetCooperativeLevel(hWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND);
	if (result != DI_OK) {
		return 0;
	}

	//acquire the mouse
	result = dimouse->Acquire();
	if (result != DI_OK) {
		return 0;
	}

	//return ok
	return 1;
}

void Input::PollMouse() {
	dimouse->GetDeviceState(sizeof(mouse_state), (LPVOID)&mouse_state);
}

int Input::MouseButton(int button) {
	return BUTTON_DOWN(mouse_state, button);
}

int Input::Mouse_X() {
	return mouse_state.lX;
}
int Input::Mouse_Y() {
	return mouse_state.lY;
}

void Input::KillMouse() {
	if (dimouse != NULL) {
		dimouse->Unacquire();
		dimouse->Release();
		dimouse = NULL;
	}
}
#pragma endregion
