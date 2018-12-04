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
		sprintf_s(s, "sprites/sun/%d.png", i);
		anim->sprite[i] = new Sprite(s);
	}

}

void Sun::update()
{
	if (dirUp == 1)
	{
		if (y >= 100)
		{
			changeDirVertical();
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
			changeDirVertical();
		}
		else
		{
			y -= 2;
		}
	}

	D3DXVECTOR2 translation = D3DXVECTOR2(x, y);
	D3DXVECTOR2 scale = D3DXVECTOR2(2, 2 * -dirUp);
	D3DXMatrixTransformation2D(&matrix, NULL, 0, &scale, NULL,
		NULL, &translation);
	MObject::update();
}