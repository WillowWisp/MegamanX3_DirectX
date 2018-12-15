#include "Sprite.h"

Sprite::Sprite()
{
	//anim = new Animation();
}


Sprite::~Sprite()
{
	//delete anim;
	if (texture != NULL) {
		texture->Release();
	}
}

Sprite::Sprite(const char* _filePath, RECT _sourceRect, int _width, int _height, D3DCOLOR _colorKey) {
	this->InitWithSprite(_filePath, _sourceRect, _width, _height, _colorKey);
}

bool Sprite::IsRect(RECT rect)
{
	if (rect.left == rect.right)
		return false;

	if (rect.top == rect.bottom)
		return false;

	return true;
}

void Sprite::InitWithSprite(const char* _filePath, RECT _sourceRect, int _width, int _height, D3DCOLOR _colorKey)
{
	position = D3DXVECTOR3(0, 0, 0);
	rotation = 0;
	rotationCenter = D3DXVECTOR2(position.x, position.y);
	translation = D3DXVECTOR2(0, 0);
	scale = D3DXVECTOR2(0, 1);
	sourceRect = _sourceRect;
	scale.x = scale.y = 1;

	isFlipHorizontal = false;
	isFlipVertical = false;

	D3DXGetImageInfoFromFile(_filePath, &imageInfo);

	if (_width == NULL) //Nếu _width không được truyền vào thì
	{
		if (!IsRect(sourceRect)) //Nếu _sourceRect không được truyền vào thì lấy width của hình
			width = imageInfo.Width;
		else
			width = sourceRect.right - sourceRect.left;
	}
	else
		width = _width;

	if (_height == NULL)
	{
		if (!IsRect(sourceRect))
			height = imageInfo.Height;
		else
			height = sourceRect.bottom - sourceRect.top;
	}
	else
		height = _height;

	if (!IsRect(sourceRect)) //Nếu _sourceRect = RECT() (giống NULL) thì lấy _width + _height
	{
		sourceRect.left = 0;
		sourceRect.right = width;
		sourceRect.top = 0;
		sourceRect.bottom = height;
	}


	D3DXCreateTextureFromFileEx(
		GameGlobal::d3ddev,
		_filePath,
		imageInfo.Width,
		imageInfo.Height,
		1,
		D3DUSAGE_DYNAMIC,
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		_colorKey,
		&imageInfo,
		NULL,
		&texture);
}

void Sprite::Draw(D3DXVECTOR3 _position, RECT _sourceRect, D3DXVECTOR2 _scale, D3DXVECTOR2 _transform,  D3DXVECTOR2 _rotationCenter)
{
	D3DXVECTOR3 inPosition = position;
	RECT inSourceRect = sourceRect;
	float inRotation = rotation;
	D3DXVECTOR2 inScale = scale;
	D3DXVECTOR2 inTranslation = translation;
	D3DXVECTOR2 inRotationCenter = rotationCenter;
	D3DXVECTOR2 scalingScenter = D3DXVECTOR2(inPosition.x, inPosition.y);

	if (_position != D3DXVECTOR3())
		inPosition = _position;

	if (IsRect(_sourceRect))
		inSourceRect = _sourceRect;

	if (_scale != D3DXVECTOR2())
		inScale = _scale;

	if (_transform != D3DXVECTOR2())
		inTranslation = _transform;

	if (_rotationCenter != D3DXVECTOR2())
		inRotationCenter = _rotationCenter;
	else
		rotationCenter = D3DXVECTOR2(inPosition.x, inPosition.y);// cho phep quay giua hinh

	D3DXMatrixTransformation2D(&matrix, &scalingScenter, 0, &inScale, &inRotationCenter,
		inRotation, &inTranslation);

	D3DXMATRIX oldMatrix;
	GameGlobal::mSpriteHandler->GetTransform(&oldMatrix);
	GameGlobal::mSpriteHandler->SetTransform(&matrix);

	D3DXVECTOR3 center = D3DXVECTOR3(width / 2, height / 2, 0);
	//D3DXVECTOR3 center = D3DXVECTOR3(width / 2, 17, 0);

	GameGlobal::mSpriteHandler->Draw(texture,
		&inSourceRect,
		&center,
		&inPosition,
		D3DCOLOR_ARGB(255, 255, 255, 255)); // nhung pixel nao co mau trang se duoc to mau nay len

	GameGlobal::mSpriteHandler->SetTransform(&oldMatrix); // set lai matrix cu~ de Sprite chi ap dung transfrom voi class nay
}

void Sprite::Draw(D3DXMATRIX _matrix, D3DCOLOR _color, D3DXVECTOR3 _center) {
	D3DXMATRIX oldMatrix;
	GameGlobal::mSpriteHandler->GetTransform(&oldMatrix);
	GameGlobal::mSpriteHandler->SetTransform(&_matrix);

	D3DXVECTOR3 center;

	if (_center != D3DXVECTOR3()) {
		center = _center;
	}
	else {
		center = D3DXVECTOR3(width / 2, height / 2, 0);
	}

	GameGlobal::mSpriteHandler->Draw(texture,
		NULL,
		&center,
		NULL,
		_color); // nhung pixel nao co mau trang se duoc to mau nay len

	GameGlobal::mSpriteHandler->SetTransform(&oldMatrix); // set lai matrix cu~ de Sprite chi ap dung transfrom voi class nay
}

void Sprite::FlipVertical() {
	scale = D3DXVECTOR2(-scale.x, scale.y);
	isFlipVertical = !isFlipVertical;
}
void Sprite::FlipHorizontal() {
	scale = D3DXVECTOR2(scale.x, -scale.y);
	isFlipHorizontal = !isFlipHorizontal;
}
//
//void Sprite::drawCurFrame() {
//	anim->animate();
//}
//
//void Sprite::setAnimState(int begin, int end, int delay) {
//	anim->beginframe = begin;
//	anim->endframe = end;
//	anim->animdelay = delay;
//	anim->animcount = 0;
//	anim->curframe = begin;
//}