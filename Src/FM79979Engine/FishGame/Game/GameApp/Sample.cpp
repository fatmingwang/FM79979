#include "stdafx.h"
//#include "stdafx.h"
#include "FishGameApp.h"
#include "../../../Core/GLSL/ToneMapping.h"
#include "../../../Core/GLSL/TunnelEffect.h"
#include "TestShader.h"

cSampleForWriteBinaryFile*g_pSampleForWriteBinaryFile = nullptr;
void	SampleForWriteBinaryFileTest()
{
	if (0)
	{
		SAFE_DELETE(g_pSampleForWriteBinaryFile);
		g_pSampleForWriteBinaryFile = new cSampleForWriteBinaryFile();
		if (g_pSampleForWriteBinaryFile)
		{
			cSampleForWriteBinaryFile::sRegister_Header_Struct l_sRegister_Header_Struct;
			l_sRegister_Header_Struct.chID[0] = 'Q';
			l_sRegister_Header_Struct.fVersion = 1.f;
			g_pSampleForWriteBinaryFile->OpenFileWhatEverFileIsExitOrNotExit("79979.zzo", l_sRegister_Header_Struct, true);
		}
		cGameApp::SoundPlay(L"MyFMBook/FMBOOK_DEMO/1.wav", true);
	}
}
void	LoadSample()
{
	SampleForWriteBinaryFileTest();
}

void	DestorySampleObject()
{
	SAFE_DELETE(g_pSampleForWriteBinaryFile);
}

void	SampleUpdate(float e_fElpaseTime)
{
	if(g_pSampleForWriteBinaryFile)
		g_pSampleForWriteBinaryFile->Update(e_fElpaseTime);
}

void	KeyboardDataRender()
{

}

void	SampleRender()
{

}

void	SampleMouseDown(int e_iPosX,int e_iPosY)
{

}

void	SampleMouseMove(int e_iPosX,int e_iPosY)
{

}

void	SampleMouseUp(int e_iPosX,int e_iPosY)
{

}

void	SampleKeyup(char e_cKey)
{
	SampleForWriteBinaryFileTest();
}