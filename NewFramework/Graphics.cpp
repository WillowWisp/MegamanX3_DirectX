#include "Graphics.h"



Graphics::Graphics()
{
}


Graphics::~Graphics()
{
}

LPDIRECT3DDEVICE9 device = NULL;

int Graphics::Init_Direct3D(HWND hWnd, int width, int height) {
	//Init Direct3D
	GameGlobal::d3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (GameGlobal::d3d == NULL) {
		MessageBox(hWnd, "Error initializing Direct3D", "Error", MB_OK);
		return 0;
	}
	//set Direct3D presentation parameters
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.Windowed = true; // Window ở dạng cửa sổ, không phải full màn hình
	d3dpp.BackBufferCount = 1; // Số lượng Back buffer
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN; // Chọn UNKNOW, DirectX sẽ tự chọn 1 định dạng phù hợp cho Device
	d3dpp.BackBufferHeight = height; // chiều cao cảu BackBuffer
	d3dpp.BackBufferWidth = width; // Chiều dài của BackBuffer
	d3dpp.hDeviceWindow = hWnd; // Gán HandleWindow cho Device
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD; // Phương thức swap của Buffer

	//create Direct3D device
	GameGlobal::d3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&device
	);

	if (device == NULL) {
		MessageBox(hWnd, "Error creating Direct3D device", "Error", MB_OK);
		return 0;
	}

	//Set current device
	GameGlobal::d3ddev = device;

	//clear the backbuffer to black
	device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	//create pointer to the back buffer
	device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &GameGlobal::backbuffer);

	return 1;
}

LPDIRECT3DSURFACE9 Graphics::LoadSurface(char *filename) {
	LPDIRECT3DSURFACE9 image = NULL;
	D3DXIMAGE_INFO info;
	HRESULT result;

	//get width and height from bitmap file
	result = D3DXGetImageInfoFromFile(filename, &info);
	if (result != D3D_OK) {
		return NULL;
	}
	//create surface
	result = GameGlobal::d3ddev->CreateOffscreenPlainSurface(
		info.Width, //width of the surface
		info.Height,
		D3DFMT_X8R8G8B8, //surface format
		D3DPOOL_DEFAULT, //memory pool to use
		&image, //pointer to the surface
		NULL //reserved (always NULL)
	);

	if (result != D3D_OK) {
		return NULL;
	}

	//load surface from file into newly created surface
	result = D3DXLoadSurfaceFromFile(
		image, //destination surface
		NULL, //destination palette
		NULL, //destination rectangle
		filename, //source filename
		NULL, //source rectangle
		D3DX_DEFAULT, //controls how image is filtered
		D3DCOLOR_XRGB(0, 0, 0), //for transparency (0 for none)
		NULL //source image info (usually NULL)
	);

	//make sure file was loaded okay
	if (result != D3D_OK) {
		return NULL;
	}
	return image;
}

LPDIRECT3DTEXTURE9 Graphics::LoadTexture(char *filename, D3DCOLOR transcolor) {
	//the texture pointer
	LPDIRECT3DTEXTURE9 texture = NULL;

	//the struct for reading bitmap file info
	D3DXIMAGE_INFO info;

	HRESULT result;
	//get width & height from bitmap file
	result = D3DXGetImageInfoFromFile(filename, &info);
	if (result != D3D_OK) {
		return NULL;
	}

	//create the new texture by loading bitmap image file
	D3DXCreateTextureFromFileEx(
		GameGlobal::d3ddev,//Direct3D device object
		filename,			//bitmap filename
		info.Width,			//bitmap img width
		info.Height,		//bitmap img height
		1,					//mip-map levels (1 for no chain)
		D3DPOOL_DEFAULT,
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		transcolor,			//color key for transparency
		&info,				//bitmap file info (from loaded file)
		NULL,				//color palette
		&texture			//destination texture
	);

	if (result != D3D_OK) {
		return NULL;
	}

	return texture;
}