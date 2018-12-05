#pragma once

#include <d3dx9.h>
#include <d3d9.h>

class Camera
{
public:
	int width, height;
	D3DXVECTOR3 position;

	Camera();
	~Camera();

	Camera(int width, int height);

	RECT GetBound(); //l?y RECT khung camera
};