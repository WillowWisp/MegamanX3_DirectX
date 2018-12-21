#pragma once

#include <stdio.h>
#include <vector>
#include <d3dx9.h>
#include <d3d9.h>

#define TRANSITION_SPEED 5
#define TRANSITION_HORIZONTALLY 0
#define TRANSITION_VERTICALLY 1
#define OUTER_CAMERA_ZONE 200
#define PLAYER_CAMERA_BOX 96

class Camera
{
public:
	bool isTransition;
	int transition_i;
	int width, height;
	D3DXVECTOR3 position;
	std::vector<RECT> cameraBorders;

	Camera();
	~Camera();

	Camera(int width, int height);

	RECT GetBound(); //lay RECT khung camera
	RECT GetPlayerCameraBox(RECT playerRect);
	void SetNextTransition_i(RECT playerRect); //set i cho RECT tiep theo (dung cho boss room)
	bool IsContain(RECT container, RECT rect);
	bool IsIntersect(RECT rect1, RECT rect2);
	bool IsInbetween(RECT containerLeft, RECT containerRight, RECT rect);
	void Reposition(RECT playerRect);
	void Transition(RECT playerRect, int dir);
	void TransitionToBossRoom();
};