#include "dxaudio.h"
//CSoudManager *dsound;

DXSound::DXSound()
{
}

int DXSound::Init_DirectSound(HWND hwnd)
{
	HRESULT result;
	//create DirectSound manager object 
	dsound = new CSoundManager();
	//initialize DirectSound 
	result = DXSound::dsound->Initialize(hwnd, DSSCL_PRIORITY);
	if (result != DS_OK) return 0;
	//set the primary buffer format 
	result = DXSound::dsound->SetPrimaryBufferFormat(2, 22050, 16);
	if (result != DS_OK) return 0;
	//return success 
	return 1;
}


CSound * DXSound::LoadSound(char *filename)
{
	HRESULT result;
	//create local reference to wave data 
	CSound *wave;
	//attempt to load the wave file
	result = dsound->Create(&wave, filename);
	if (result != DS_OK)
		return NULL;
	//return the wave 
	return wave;
}

void DXSound::PlaySound(CSound *sound)
{
	sound->Play();
}

void DXSound::LoopSound(CSound *sound)
{
	sound->Play(0, DSBPLAY_LOOPING);
}

void DXSound::StopSound(CSound *sound)
{
	sound->Stop();
}
