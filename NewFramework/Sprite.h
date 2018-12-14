#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include "GameGlobal.h"

class Sprite
{
public:
	D3DXIMAGE_INFO          imageInfo; // thong tin thuc cua hinh anh duoc lay
	LPDIRECT3DTEXTURE9      texture; // load hinh anh vao day
	D3DXMATRIX              matrix; // ma tran cua Spite ho tro trong cac phep hinh hoc (giống matrixCombined)

	D3DXVECTOR3             position; // vi tri cua Sprite, <<CÓ GỐC LÀ CHÍNH GIỮA>> hinh anh Texture
	RECT                    sourceRect; // hinh chu nhat cat tu anh cua texture

	int                     width, height; // kich thuoc cua texture

	bool                    isFlipVertical, // lat theo chieu doc
							isFlipHorizontal; // lat theo chieu ngang

	float                   rotation; // goc quay cua Sprite tinh theo radian

	D3DXVECTOR2             scale; // Phóng to / Thu nhỏ sprite
	D3DXVECTOR2             translation; // Dời sprite
	D3DXVECTOR2             rotationCenter; // diem trung tam trong phep xoay hinh (origin vector) (chưa hỉu lắm??)

	Sprite();
	~Sprite();

	Sprite(const char* _filePath, RECT _sourceRect = RECT(), int _width = NULL, int _height = NULL, D3DCOLOR _colorKey = D3DCOLOR_XRGB(255, 0, 255));

	//Sử dụng cho kế thừa
	void InitWithSprite(const char* _filePath, RECT _sourceRect = RECT(), int _width = NULL, int _height = NULL, D3DCOLOR _colorKey = D3DCOLOR_XRGB(255, 0, 255));
	bool IsRect(RECT rect);


	//Hàm chính để vẽ sprite
	void Draw(D3DXVECTOR3 _position = D3DXVECTOR3(), RECT _sourceRect = RECT(), D3DXVECTOR2 _scale = D3DXVECTOR2(), D3DXVECTOR2 _transform = D3DXVECTOR2(), D3DXVECTOR2 _rotationCenter = D3DXVECTOR2());
	//Hàm thêm vào để rút gọn số parameters cần nhập
	void Draw(D3DXMATRIX _matrix, D3DCOLOR _color = D3DCOLOR_ARGB(255, 255, 255, 255), D3DXVECTOR3 _center = D3DXVECTOR3());

	void FlipVertical(); //Lật quanh trục dọc
	void FlipHorizontal(); //Lật quanh trục ngang

};

