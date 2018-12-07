#include "Camera.h"



Camera::Camera()
{
}


Camera::~Camera()
{
}

Camera::Camera(int _width, int _height)
{
	width = _width;
	height = _height;

	position = D3DXVECTOR3(0, 0, 0);
}

RECT Camera::GetBound() {
	RECT bound;

	bound.left = position.x - width / 2;
	bound.right = bound.left + width;;
	bound.top = position.y - height / 2;
	bound.bottom = bound.top + height;

	return bound;
}