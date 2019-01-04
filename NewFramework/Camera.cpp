#include "Camera.h"



Camera::Camera()
{
	isTransition = false;
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

RECT Camera::GetPlayerCameraBox(RECT playerRect) {
	RECT cameraBox;

	cameraBox.left = playerRect.left - PLAYER_CAMERA_BOX / 2 + (playerRect.right - playerRect.left) / 2;
	cameraBox.right = cameraBox.left + PLAYER_CAMERA_BOX;
	//cameraBox.top = playerRect.top - PLAYER_CAMERA_BOX / 2 + (playerRect.bottom - playerRect.top) / 2;
	//cameraBox.bottom = cameraBox.top + PLAYER_CAMERA_BOX;
	cameraBox.bottom = playerRect.bottom;
	cameraBox.top = cameraBox.bottom - PLAYER_CAMERA_BOX;

	return cameraBox;
}

bool Camera::IsContain(RECT container, RECT rect) {
	if (rect.left >= container.left && rect.right <= container.right && rect.top >= container.top && rect.bottom <= container.bottom)
	{
		return true;
	}

	return false;
}

bool Camera::IsIntersect(RECT rect1, RECT rect2) {
	if (rect1.left > rect2.right || rect1.right < rect2.left || rect1.top > rect2.bottom || rect1.bottom < rect2.top)
	{
		return false;
	}

	return true;
}

bool Camera::IsInbetween(RECT containerLeft, RECT containerRight, RECT rect) {
	if (IsIntersect(containerLeft, rect)
		&& IsIntersect(containerRight, rect)
		&& containerLeft.left < containerRight.left) {
		return true;
	}

	return false;
}

void Camera::Reposition(RECT playerRect) {
	for (int i = 0; i < cameraBorders.size(); i++) {
		if (i < cameraBorders.size() - 1
			&& IsContain(cameraBorders[i], playerRect)
			&& IsContain(cameraBorders[i + 1], playerRect)) {

			isTransition = true;
			transition_i = i + 1;
			if (cameraBorders[i].right - cameraBorders[i].left > cameraBorders[i].bottom - cameraBorders[i].top) {
				Transition(playerRect, TRANSITION_HORIZONTALLY);
			}
			else {
				Transition(playerRect, TRANSITION_VERTICALLY);
			}
			return;
		}
		else if (IsContain(cameraBorders[i], playerRect)) {
			//RECT playerCameraBox = GetPlayerCameraBox(playerRect);
			//position.x = playerCameraBox.left + (playerCameraBox.right - playerCameraBox.left) / 2;
			//position.y = playerCameraBox.top + (playerCameraBox.bottom - playerCameraBox.top) / 2;

			position.x = playerRect.left + (playerRect.right - playerRect.left) / 2;
			position.y = playerRect.top + (playerRect.bottom - playerRect.top) / 2;

			RECT bound = GetBound();

			if (bound.left < cameraBorders[i].left) {
				/*position.x = cameraBorders[i].left + (cameraBorders[i].right - cameraBorders[i].left) / 2;*/
				position.x = cameraBorders[i].left + width / 2;
				//position.x++;
			}
			if (bound.right > cameraBorders[i].right) {
				position.x = cameraBorders[i].right - width / 2;
			}
			if (bound.bottom > cameraBorders[i].bottom) {
				position.y = cameraBorders[i].bottom - height / 2;
				//position.y++;
			}
			if (bound.top < cameraBorders[i].top) {
				position.y = cameraBorders[i].top + height / 2;
				//position.y--;
			}
			//break;
		}
	}
}

void Camera::Transition(RECT playerRect, int dir) {
	RECT bound = GetBound();
	RECT destRect = cameraBorders[transition_i];

	RECT boundCentered; //Bound that has player in the center
	boundCentered.left = playerRect.left - (width / 2 - (playerRect.right - playerRect.left) / 2);
	boundCentered.top = playerRect.top - (height / 2 - (playerRect.bottom - playerRect.top) / 2);
	boundCentered.right = boundCentered.left + width;
	boundCentered.bottom = boundCentered.top + height;


	if (IsContain(destRect, bound)) {
		isTransition = false;
		cameraBorders.erase(cameraBorders.begin() + transition_i - 1);
		transition_i = -1;
		return;
	}

	if (dir == TRANSITION_HORIZONTALLY) {
		if (boundCentered.left > bound.left
			&& boundCentered.right <= destRect.right) {
			position.x = boundCentered.left + (boundCentered.right - boundCentered.left) / 2;
		}
		else {
			if (bound.left < destRect.left) {
				position.x += TRANSITION_SPEED;
			}
			else if (bound.right > destRect.right) {
				position.x -= TRANSITION_SPEED;
			}
		}

		if (bound.top < destRect.top) {
			position.y = destRect.top + height / 2;
			//position.y--;
		}
		else if (bound.bottom > destRect.bottom) {
			position.y = destRect.bottom - height / 2;
			//position.y++;
		}
	}
	else {
		int dirUp = cameraBorders[transition_i - 1].top - destRect.top;

		if (dirUp > 0 && boundCentered.bottom < bound.bottom
			&& boundCentered.top >= destRect.top) {
			position.y = boundCentered.bottom - (boundCentered.bottom - boundCentered.top) / 2;
		}
		else if (dirUp < 0 && boundCentered.top > bound.top
			&& boundCentered.bottom <= destRect.bottom) {
			position.y = boundCentered.top + (boundCentered.bottom - boundCentered.top) / 2;
		}
		else {
			if (bound.bottom > destRect.bottom) {
				position.y -= TRANSITION_SPEED;
			}
			else if (bound.top < destRect.top) {
				position.y += TRANSITION_SPEED;
			}
		}

		if (bound.left < destRect.left) {
			position.x = destRect.left + width / 2;
		}
		else if (bound.right > destRect.right) {
			position.x = destRect.right - width / 2;
		}
	}
}

void Camera::SetNextTransition_i(RECT playerRect) {
	for (int i = 0; i < cameraBorders.size() - 1; i++) {
		if (IsContain(cameraBorders[i], playerRect)) {
			transition_i = i + 1;
			break;
		}
	}
}

void Camera::TransitionToBossRoom() {
	if (isTransition == false)
		return;

	RECT bound = GetBound();
	RECT destRect = cameraBorders[transition_i];


	if (bound.left >= destRect.left) {
		isTransition = false;
		cameraBorders.erase(cameraBorders.begin() + transition_i - 1);
		transition_i = -1;
		return;
	}
	else {
		position.x += TRANSITION_SPEED;
	}
}

