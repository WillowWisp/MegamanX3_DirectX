#include "DebugDraw.h"

DebugDraw::DebugDraw()
{
	mSpriteHandler = GameGlobal::mSpriteHandler;
	mSpriteHandler->GetDevice(&mddv);
	D3DXCreateLine(mddv, &LineDraw);

	//set size cho line
	LineDraw->SetWidth(5);

	//set mau cho line
	mColor = D3DCOLOR_XRGB(255, 0, 0);

}

DebugDraw::~DebugDraw()
{

}

void DebugDraw::Draw()
{

}

void DebugDraw::setLineSize(float width)
{
	LineDraw->SetWidth(width);
}

void DebugDraw::DrawLine(D3DXVECTOR2 lines[], int count)
{
	LineDraw->Begin();
	LineDraw->Draw(lines, count, mColor);
	LineDraw->End();
}

void DebugDraw::DrawRect(RECT rect, Camera *camera)
{

	D3DXVECTOR3 trans = D3DXVECTOR3(0, 0, 0);

	if (camera)
	{
		trans = D3DXVECTOR3(GameGlobal::wndWidth / 2, GameGlobal::wndHeight / 2, 0) - camera->position;
	}

	D3DXVECTOR2 lines[] = { D3DXVECTOR2(rect.left + trans.x, rect.top + trans.y),
							D3DXVECTOR2(rect.right + trans.x, rect.top + trans.y),
							D3DXVECTOR2(rect.right + trans.x, rect.bottom + trans.y),
							D3DXVECTOR2(rect.left + trans.x, rect.bottom + trans.y),
							D3DXVECTOR2(rect.left + trans.x, rect.top + trans.y) };

	DrawLine(lines, 5);
}

void DebugDraw::setColor(D3DCOLOR color)
{
	mColor = color;
}

D3DCOLOR DebugDraw::getColor()
{
	return mColor;
}