#include "Sun.h"

Sun::Sun(int X, int Y)
{
	x = X;
	y = Y;
	movex = 0;
	movey = 0;
	dirUp = 1;
	dirRight = 1;
	anim = new Animation(6, 0, 5, 10);

	char s[50];
	for (int i = 0; i < 6; i++) {
		sprintf_s(s, "sprites/sun/%d.bmp", i);
		anim->sprite[i] = new Sprite(s);
	}

}

void Sun::Update()
{
	if (dirUp == 1)
	{
		if (y >= 100)
		{
			ChangeDirVertical();
		}
		else
		{
			y += 5;
		}
	}
	else
	{
		if (y <= 0)
		{
			ChangeDirVertical();
		}
		else
		{
			y -= 2;
		}
	}

	D3DXVECTOR2 translation = D3DXVECTOR2(x, y);
	D3DXVECTOR2 scale = D3DXVECTOR2(0.5, 200 * -dirUp);
	D3DXMatrixTransformation2D(&matrix, NULL, 0, &scale, NULL,
		NULL, &translation);
	MObject::Update();
}