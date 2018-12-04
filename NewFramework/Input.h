#pragma once
#ifndef _DXINPUT_H
#define _DXINPUT_H

#include <dinput.h>

//FILE INPUT TẠM ĐỂ KIỂM TRA THAY ĐỔI STATE CỦA MEGAMAN, SẼ BỎ ĐI KHI MERGE

//function prototypes
int Init_DirectInput(HWND);

int Init_Keyboard(HWND);
void Poll_Keyboard();
int Key_Down(int);
void Kill_Keyboard();

int Init_Mouse(HWND);
void Poll_Mouse();
int Mouse_Button(int);
int Mouse_X();
int Mouse_Y();
void Kill_Mouse();

//DirectInput objects, devices and states
extern LPDIRECTINPUT8 dinput;
extern LPDIRECTINPUTDEVICE8 dimouse;
extern LPDIRECTINPUTDEVICE8 dikeyboard;
extern DIMOUSESTATE mouse_state;

#endif