#ifdef WIN32

#include "../stdafx.h"
#include "AVIPlayer.h"
#include "../Image/SimplePrimitive.h"
#include "../GLSL/Shader.h"
#include "../Sound/BasicSound.h"
#include "../Image/Texture.h"
#include "../GameplayUT/GameApp.h"
//#pragma comment(lib,"ole32.lib")
//#define	MAX_PIXELS	512
#ifndef IOS//iphone platform
//LU	RU
//
//LD	RD

#define	UV_LU  glTexCoord2f(0.0,0.0);
#define	UV_LD  glTexCoord2f(0.0,1.0);
#define	UV_RD  glTexCoord2f(1.0,1.0);
#define	UV_RU  glTexCoord2f(1.0,0.0);
//texCoordinate is a array that have 4 capticy,left,top,right,bottom
#define	DRAW_QUAD(width,height)	UV_LU glVertex2i(-width,-height);/*0*/  UV_LD glVertex2i(-width,height);/*1*/	  UV_RD	glVertex2i(width,height);/*2*/  UV_RU	glVertex2i(width,-height);/*3*/
#define	DRAW_QUAD_MIRROR(width,height)	UV_RU	glVertex2i(-width,-height);/*0*/UV_RD	glVertex2i(-width,height);/*1*/UV_LD glVertex2i(width,height);/*2*/UV_LU glVertex2i(width,-height);/*3*/
#define	DRAW_QUAD_BY_TEX_COORDINATE(texCoordinate,Width,Height)glTexCoord2f(texCoordinate[0],texCoordinate[1]); glVertex2i(-Width,-Height);/*0*/glTexCoord2f(texCoordinate[0],texCoordinate[3]); glVertex2i(-Width, Height);/*1*/glTexCoord2f(texCoordinate[2],texCoordinate[3]); glVertex2i( Width, Height);/*2*/glTexCoord2f(texCoordinate[2],texCoordinate[1]); glVertex2i( Width,-Height);/*3*/
#define	DRAW_QUAD_BY_TEX_COORDINATE_MIRROR(texCoordinate,Width,Height)glTexCoord2f(texCoordinate[2],texCoordinate[1]);glVertex2i(-Width,-Height);/*0*/glTexCoord2f(texCoordinate[2],texCoordinate[3]);glVertex2i(-Width, Height);/*1*/glTexCoord2f(texCoordinate[0],texCoordinate[3]);glVertex2i( Width, Height);/*2*/glTexCoord2f(texCoordinate[0],texCoordinate[1]);glVertex2i( Width,-Height);/*3*/
#endif
void flipIt(void* buffer,int e_iWidth,int e_iHeight)			// Flips The Red And Blue Bytes (256x256)
{
	char	*l_Data = (char*)buffer;
	for( int i=0;i<e_iWidth;++i )
	{
		for( int j=0;j<e_iHeight;++j )
		{
			char*	l_Color = &l_Data[(j*e_iWidth+i)*3];//RGB
			char	l_Red = l_Color[0];
			char	l_Blue = l_Color[2];
			l_Color[0] = l_Blue;
			l_Color[2] = l_Red;
		}
	}

	//void* b = buffer;											// Pointer To The Buffer
	//__asm														// Assembler Code To Follow
	//{
	//	mov ecx, e_iWidth*e_iHeight							// Counter Set To Dimensions Of Our Memory Block
	//	mov ebx, b												// Points ebx To Our Data (b)
	//	label:													// Label Used For Looping
	//		mov al,[ebx+0]										// Loads Value At ebx Into al
	//		mov ah,[ebx+2]										// Loads Value At ebx+2 Into ah
	//		mov [ebx+2],al										// Stores Value In al At ebx+2
	//		mov [ebx+0],ah										// Stores Value In ah At ebx
	//
	//		add ebx,3											// Moves Through The Data By 3 Bytes
	//		dec ecx												// Decreases Our Loop Counter
	//		jnz label											// If Not Zero Jump Back To Label
	//}
}
#ifdef WIN32
cAVIPlayer::cAVIPlayer()
{
	m_iPrior = -1;
	m_iCurrentAudioStep = 0;
	m_pBasicSound = 0;
	m_iAudioStreamLength = 0;
	m_dbTimeLength = 0;
	m_iNext = 0;
	m_iFrame = 0;
	m_lLastframe = 0;
	m_bLoop = false;
	m_bDone = false;
	m_pCurrentTextureData = 0;
	m_uiTex_id = 0;
	m_PGF = 0;
	m_hdc = CreateCompatibleDC(0);
	m_HDD = DrawDibOpen();			// Grab A Device Context For Our Dib
	m_hBitmap = 0;
	m_pDIBData = 0;
	m_pAviFile = 0;
	m_pAVIStreamVideo = 0;
	m_pAVIStreamSound = 0;
	m_fFPS = 0;
}

cAVIPlayer::~cAVIPlayer()
{
//	DrawDibClose(m_hdc);										// Closes The DrawDib Device Context
	CloseFile();
	//DrawDibClose(m_hdc);										// Closes The DrawDib Device Context
	cGameApp::OutputDebugInfoString(L"FUCK I am lazy to debug this...why it occur crash DrawDibClose?");
	m_hdc = 0;
}

void	cAVIPlayer::SetCurrentFrame(int e_iFrame)
{
	m_iNext = m_iMPF*e_iFrame;
}

double	cAVIPlayer::GetFileTimeLength()
{
	return m_dbTimeLength;
}

void	cAVIPlayer::SetCurrentFrameByTime(float e_fTime)
{
	//get lerp value
	double	l_fLerp = e_fTime/this->m_dbTimeLength;
	//find the frame as we need
	m_iNext  = (int)(m_lLastframe*l_fLerp*m_iMPF);
}

void	cAVIPlayer::CloseFile()
{
	m_dbTimeLength = 0;
	m_iNext = 0;
	m_iFrame = 0;
	m_lLastframe = 0;
	m_pCurrentTextureData = 0;
	m_bDone = false;
	if( m_hBitmap )
	{
		DeleteObject(m_hBitmap);									// Delete The Device Dependant Bitmap Object
		m_hBitmap =0 ;
	}
	if( m_pAviFile )
	{
		AVIFileClose(m_pAviFile);
		m_pAviFile = 0;
	}
	if( m_PGF )
	{
		AVIStreamGetFrameClose(m_PGF);								// Deallocates The GetFrame Resources
		m_PGF = 0;
	}
	if( m_pAVIStreamVideo )
	{
		AVIStreamRelease(m_pAVIStreamVideo);					// Release The Stream
		m_pAVIStreamVideo = 0;
	}
	if( m_pAVIStreamSound )
	{
		AVIStreamRelease(m_pAVIStreamSound);					// Release The Stream
		m_pAVIStreamSound = 0;
	}
	AVIFileExit();												// Release The File
	if( m_uiTex_id != 0 )
	{
		glDeleteTextures(1,&m_uiTex_id);
		m_uiTex_id = 0;
	}
}

void	cAVIPlayer::ParseVideoStream()
{
	HRESULT result;
	// extract AVI video stream
	result = AVIFileGetStream(m_pAviFile, &m_pAVIStreamVideo, streamtypeVIDEO, 0);
	if(FAILED(result))
	{
		UT::ErrorMsg(L"no video exist",L"warning");
		return;
	}
	AVIStreamInfo(m_pAVIStreamVideo, &m_PSI, sizeof(m_PSI));						// Reads Information About The Stream Into psi
	m_iWidth=m_PSI.rcFrame.right-m_PSI.rcFrame.left;					// Width Is Right Side Of Frame Minus Left
	m_iHeight=m_PSI.rcFrame.bottom-m_PSI.rcFrame.top;					// Height Is Bottom Of Frame Minus Top
	
	m_lLastframe = AVIStreamLength(m_pAVIStreamVideo);								// The Last Frame Of The Stream

	m_iMPF = AVIStreamSampleToTime(m_pAVIStreamVideo,m_lLastframe)/m_lLastframe;		// Calculate Rough Milliseconds Per Frame

	m_fFPS = 1/(float)m_iMPF*1000;

	m_dbTimeLength = (m_PSI.dwLength)/m_fFPS;

	//BITMAPINFOHEADER bminfo;
	//m_Bmih.biWidth = vidsinfo.rcFrame.right - vidsinfo.rcFrame.left;
	//m_Bmih.biHeight = vidsinfo.rcFrame.bottom - vidsinfo.rcFrame.top;
	//m_Bmih.biBitCount = 16;
	m_Bmih.biCompression = BI_RGB;
	m_Bmih.biSizeImage = 0;
	m_Bmih.biXPelsPerMeter = 0;
	m_Bmih.biYPelsPerMeter = 0;
	m_Bmih.biClrUsed = 0;
	m_Bmih.biClrImportant = 0;

	m_Bmih.biSize = sizeof (BITMAPINFOHEADER);							// Size Of The BitmapInfoHeader
	m_Bmih.biPlanes = 1;												// Bitplanes
	m_Bmih.biBitCount = 24;												// Bits Format We Want (24 Bit, 3 Bytes)
	m_Bmih.biWidth = m_iWidth;										// Width We Want (256 Pixels)
	m_Bmih.biHeight = m_iHeight;										// Height We Want (256 Pixels)
	m_Bmih.biCompression = BI_RGB;										// Requested Mode = RGB

	m_hBitmap = CreateDIBSection (m_hdc, (BITMAPINFO*)(&m_Bmih), DIB_RGB_COLORS, (void**)(&m_pDIBData), nullptr, 0);
	SelectObject (m_hdc, m_hBitmap);									// Select hBitmap Into Our Device Context (hdc)

	m_PGF = AVIStreamGetFrameOpen(m_pAVIStreamVideo, &m_Bmih);							// Create The PGETFRAME	Using Our Request Mode
}

void	cAVIPlayer::ParseAudioStream()
{
	SAFE_DELETE(m_pBasicSound);
	HRESULT result;
	// extract AVI audio stream
	result = AVIFileGetStream(m_pAviFile, &m_pAVIStreamSound, streamtypeAUDIO, 0);
	if(FAILED(result))
	{
		UT::ErrorMsg(L"no sound exist",L"warning");
		//return false;
		//continue;
	}
	else
	{
		long l_iSize = 0;
		AVIStreamInfo(m_pAVIStreamSound, &m_PSIAudio, sizeof(m_PSIAudio));						// Reads Information About The Stream Into psi
		if(AVIStreamReadFormat(m_pAVIStreamSound, AVIStreamStart(m_pAVIStreamSound), 0, &l_iSize))
		{
			UT::ErrorMsg(L"read sound format error",L"warning");
			SAFE_DELETE(m_pBasicSound);
		}
		else
		{
			byte* pChunk = new byte[l_iSize];
			if(AVIStreamReadFormat(m_pAVIStreamSound, AVIStreamStart(m_pAVIStreamSound), pChunk, &l_iSize))
			{
			   UT::ErrorMsg(L"read sound format chunk error",L"warning");
			}
			LPWAVEFORMAT pWaveFormat = (LPWAVEFORMAT)pChunk;
			//data compressed,I am lazy fo decoder...
			if(pWaveFormat->wFormatTag !=1)
			{
				SAFE_DELETE(m_pBasicSound);
			}
			if( m_pBasicSound )
			{
				m_iAudioStreamLength = AVIStreamLength(m_pAVIStreamSound);
				m_iCurrentAudioStep = MAX_AUDIOQ_SIZE;
				if( m_iAudioStreamLength < m_iCurrentAudioStep )
				{
					m_iCurrentAudioStep = m_iAudioStreamLength;
				}
				//1 byte 8 bit
				m_PerSampleBit = pWaveFormat->nBlockAlign*8;
				int	l_iFormat = m_pBasicSound->PerSampleBitAndChannelToOpenALFormat(m_PerSampleBit,pWaveFormat->nChannels);
				m_pBasicSound->SetFreq(pWaveFormat->nSamplesPerSec);
				m_pBasicSound->SetSize(m_iCurrentAudioStep);
				m_pBasicSound->SetFormat(l_iFormat);
				SetAudioPosByTime(0.f);
			}
			delete pChunk;
		}
	}
}

void	cAVIPlayer::UpdateVideo(float e_fElpaseTime)
{
	if( !m_PGF )
		return;
	GrabAVIFrame(m_iFrame);	
}

void	cAVIPlayer::SetAudioPosByTime(float e_fTime)
{
	if(!m_pBasicSound)
		return;
	double	l_fTarget = e_fTime/m_dbTimeLength;
	if( l_fTarget >=1.0f || l_fTarget < 0.f )
	{
		if(m_pBasicSound)
		{
			m_pBasicSound->Play(false);
			return;
		}
	}
	int	l_iTarget = (int)(m_iAudioStreamLength*l_fTarget);
	int	l_iSize = MAX_AUDIOQ_SIZE;
	m_iCurrentAudioStep = l_iTarget;
	if( m_iAudioStreamLength < m_iCurrentAudioStep )
	{
		l_iSize = m_iAudioStreamLength - m_iCurrentAudioStep;
		m_iCurrentAudioStep = m_iAudioStreamLength;
	}
	m_pBasicSound->SetSize(l_iSize);
	byte*l_pBuffer = new byte[l_iSize*m_PerSampleBit];
	if(!l_pBuffer)
	{
		UT::ErrorMsg(L"out of memory",L"warning");
		return;
	}
	if(AVIStreamRead(m_pAVIStreamSound, m_iCurrentAudioStep, AVISTREAMREAD_CONVENIENT, l_pBuffer, l_iSize, 0,0))
	{
		UT::ErrorMsg(L"read of memory",L"warning");
		return;
	}
	m_pBasicSound->AssignBufferData(l_pBuffer);
	delete l_pBuffer;
	m_pBasicSound->Play(true);
}

void	cAVIPlayer::UpdateSound(float e_fElpaseTime)
{
	if( !m_pBasicSound )
		return;
	if(m_pBasicSound->IsPlay())
		return;
	int	l_iTarget = m_iCurrentAudioStep + MAX_AUDIOQ_SIZE;
	if( m_iCurrentAudioStep >= m_iAudioStreamLength )
	{
		m_iCurrentAudioStep = 0;
		//it's end
		if(!this->m_bLoop)
		{
			return;
		}
		SetAudioPosByTime(0.f);
	}
	else
	{
		m_iCurrentAudioStep = l_iTarget;
		float	l_fTime = m_iCurrentAudioStep/(float)m_iAudioStreamLength;
		SetAudioPosByTime(l_fTime);
	}
}

bool cAVIPlayer::OpenFile(const wchar_t*e_strFileName)
//void cAVIPlayer::OpenFile(wchar_t*e_strFileName)				// Opens An AVI File (szFile)
{
	//CoInitialize(nullptr);
	CloseFile();
	if( m_uiTex_id == 0 )
		glGenTextures( 1, &m_uiTex_id );
	else
	{
		glDeleteTextures(1,&m_uiTex_id);
		glGenTextures( 1, &m_uiTex_id );
	}

	AVIFileInit();												// Opens The AVIFile Library

	// open AVI file
	HRESULT result = AVIFileOpen(&m_pAviFile, e_strFileName, OF_READ, 0);
	if(FAILED(result))
	{
		UT::ErrorMsg(L"openfile eror",e_strFileName);
		return false;
	}

	// extract AVI file information
	AVIFILEINFO fileinfo;
	result = AVIFileInfo(m_pAviFile, &fileinfo, sizeof(fileinfo));
	if(FAILED(result))
	{
		UT::ErrorMsg(L"aviinfo eror",e_strFileName);
		return false;
	}
	ParseVideoStream();
	ParseAudioStream();
	//m_PGF = AVIStreamGetFrameOpen(m_pAVIStreamVideo, 0);							// Create The PGETFRAME	Using Our Request Mode

	if( m_PGF == nullptr )
	{
		// An Error Occurred Opening The Frame
		MessageBox (HWND_DESKTOP, L"Failed To Open The AVI Frame", L"Error", MB_OK | MB_ICONEXCLAMATION);
		CloseFile();
		return false;
	}
	glEnable(GL_TEXTURE_2D);											// Enable Texture Mapping
	glBindTexture( GL_TEXTURE_2D, m_uiTex_id);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);	// Set Texture Max Filter
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);	// Set Texture Min Filter

	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);				// Set The Texture Generation Mode For S To Sphere Mapping
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);				// Set The Texture Generation Mode For T To Sphere Mapping
	// Create The Texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_iWidth, m_iHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, m_pDIBData);
	return true;
}

void cAVIPlayer::GrabAVIFrame(int frame)								// Grabs A Frame From The Stream
{
	LPBITMAPINFOHEADER lpbi;											// Holds The Bitmap Header Information
	lpbi = (LPBITMAPINFOHEADER)AVIStreamGetFrame(m_PGF, frame);			// Grab Data From The AVI Stream
	if( lpbi )
	{
		m_pCurrentTextureData = (char *)lpbi+lpbi->biSize+lpbi->biClrUsed * sizeof(RGBQUAD);	// Pointer To Data Returned By AVIStreamGetFrame

		// Convert Data To Requested Bitmap Format
		DrawDibDraw (m_HDD, m_hdc, 0, 0, m_iWidth, m_iHeight, lpbi, m_pCurrentTextureData, 0, 0, m_iWidth, m_iHeight, 0);

		//instead by
			  // look at the GL_BGR (flips BGR video to RGB texture)
			  //glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGB, TEX_WIDTH, TEX_HEIGHT, 0,
					//		 GL_BGR, GL_UNSIGNED_BYTE, ui_image_copy);
		//flipIt(m_pDIBData,m_iWidth,m_iHeight);							// Swap The Red And Blue Bytes (GL Compatability)
	}
}

void	cAVIPlayer::Update(float e_fElpaseTime)
{
	if( m_bPause || m_bDone )
		return;
	if( !m_PGF )
		return;
	UpdateVideo(e_fElpaseTime);
	UpdateSound(e_fElpaseTime);
	m_iNext+=(int)(e_fElpaseTime*1000);								// Increase next Based On The Timer
	m_iFrame = m_iNext/m_iMPF;										// Calculate The Current Frame
	if( m_iPrior != m_iNext )
	{
		m_iPrior = m_iNext;
		if (m_iFrame>=m_lLastframe)										// Are We At Or Past The Last Frame?
		{
			if( m_bLoop )
			{
				m_iFrame=0;												// Reset The Frame Back To Zero (Start Of Video)
				m_iNext=0;												// Reset The Animation Timer (next)
			}
			else
				m_bDone = true;
		}
	}
}
void	cAVIPlayer::Render(float e_fPosX,float e_fPosY,POINT e_Size)
{
	if( !m_PGF )
		return;
	cTexture::m_suiLastUsingImageIndex = m_uiTex_id;
	glBindTexture( GL_TEXTURE_2D, m_uiTex_id);
	//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);	// Set Texture Max Filter
	//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);	// Set Texture Min Filter
	//glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);				// Set The Texture Generation Mode For S To Sphere Mapping
	//glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);				// Set The Texture Generation Mode For T To Sphere Mapping
	UseShaderProgram(DEFAULT_SHADER);
	//color space is different
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_iWidth, m_iHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, m_pDIBData);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_iWidth, m_iHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, m_pDIBData);
	float	l_UV[] = {0,1,1,0};
	DrawQuadWithTextureAndColorAndCoordinate(e_fPosX,e_fPosY,0.f,(float)e_Size.x,(float)e_Size.y,Vector4::One,l_UV,Vector3::Zero);

}

void	cAVIPlayer::RenderByTime(float e_fTime,float e_fPosX,float e_fPosY,POINT e_Size)
{
	this->SetCurrentFrameByTime(e_fTime);
	Update(0.f);
	Render(e_fPosX,e_fPosY,e_Size);
}

bool	cAVIPlayer::IsFileOpen()
{
	return m_PGF?true:false;
}

#else//linux
cAVIPlayer::cAVIPlayer()
{
	m_iCurrentAudioStep = 0;
	m_pBasicSound = 0;
	m_iAudioStreamLength = 0;
	m_iNext = 0;
	m_iFrame = 0;
	m_iNext = 0;
	m_lLastframe = 0;
	m_bLoop = false;
	m_bDone = false;
	m_pCurrentTextureData = 0;
	m_uiTex_id = 0;
	m_pAvifile = 0;
	m_pAvistream = 0;
	m_pAudiostream = 0;
	m_pStreaminfo = 0;
	m_pImage = 0;
	m_fFPS = 0;
}

cAVIPlayer::~cAVIPlayer()
{
	SAFE_DELETE(m_pAvifile);
}

void	cAVIPlayer::CloseFile()
{
	SAFE_DELETE(m_pCurrentTextureData);
	m_iFrame = 0;
	m_bDone = false;
	m_pImage = 0;
	if( m_pAvifile )
	{
		m_pAvistream->StopStreaming();
		//m_pAudiostream->StopStreaming();
	}
	SAFE_DELETE(m_pAvifile);
	if( m_uiTex_id != 0 )
	{
		glDeleteTextures(1,&m_uiTex_id);
		m_uiTex_id = 0;
	}
}

void	cAVIPlayer::SetCurrentFrame(int e_iFrame)
{
    unsigned int l_uiPrevKeyFrame = m_pAvistream->GetPrevKeyFrame(e_iFrame);
    m_pAvistream->Seek (l_uiPrevKeyFrame);
    while( m_pAvistream->GetPos ()<e_iFrame&&!m_pAvistream->Eof () )
		m_pAvistream->ReadFrame();
}

void	cAVIPlayer::SetCurrentFrameByTime(float e_fTime)
{
	//translate time to frame
    int	l_iFrame = m_fFPS*e_fTime;
    //int	l_iFrame = (int)((e_fTime*2/m_dbTimeLength)*m_lLastframe);
    SetCurrentFrame(l_iFrame);
}

bool	cAVIPlayer::OpenFile(wchar_t*e_strFileName)
{
	CloseFile();
	if( m_uiTex_id == 0 )
		glGenTextures( 1, &m_uiTex_id );
	else
	{
		glDeleteTextures(1,&m_uiTex_id);
		glGenTextures( 1, &m_uiTex_id );
	}
	m_pAvifile = avm::CreateReadFile(UT::WcharToChar(e_strFileName));
    if (m_pAvifile->IsOpened())
	{
		m_pAvistream = m_pAvifile->GetStream(0, avm::IReadStream::Video );
		//m_pAudiostream = m_pAvifile->GetStream(0, avm::IReadStream::Audio );
	}
	else
	{
		SAFE_DELETE(m_pAvifile);
		return false;
	}
    m_pAvistream->StartStreaming();
    m_pStreaminfo = m_pAvistream->GetStreamInfo();
    m_iWidth      = m_pStreaminfo->GetVideoWidth();
    m_iHeight     = m_pStreaminfo->GetVideoHeight();
	m_pCurrentTextureData = new char[m_iWidth*m_iHeight*3];//for RGB 3 unit color
    m_dbTimeLength = m_pStreaminfo->GetLengthTime();
	m_lLastframe  = m_pStreaminfo->GetMaxFrameSize();
	m_fFPS		  = m_pStreaminfo->GetFps();
	m_iMPF		  = (int)(1000/m_fFPS);


	glEnable(GL_TEXTURE_2D);											// Enable Texture Mapping
	glBindTexture( GL_TEXTURE_2D, m_uiTex_id);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);	// Set Texture Max Filter
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);	// Set Texture Min Filter

	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);				// Set The Texture Generation Mode For S To Sphere Mapping
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);				// Set The Texture Generation Mode For T To Sphere Mapping
	// Create The Texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_iWidth, m_iHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

	return true;
}

void	cAVIPlayer::Update(float e_fElpaseTime)
{
	m_iNext+=(int)(e_fElpaseTime*1000);								// Increase next Based On The Timer
	m_iFrame = m_iNext/m_iMPF;										// Calculate The Current Frame
	if (m_iFrame>=m_lLastframe)										// Are We At Or Past The Last Frame?
	{
		if( m_bLoop )
		{
			m_iFrame=0;												// Reset The Frame Back To Zero (Start Of Video)
			m_iNext=0;												// Reset The Animation Timer (next)
			m_pAvistream->Seek(0);									// back to start to stream
		}
		else
		{
			m_bDone = true;
		}
	}
	else
	{
        //here to avoid same frame capture
        static int  l_siMPF  = 0;
        l_siMPF+=e_fElpaseTime*1000;
        if( l_siMPF>=this->m_iMPF )
        {
            while(l_siMPF>=m_fFPS)
                l_siMPF-=m_fFPS;
            m_pAvistream->ReadFrame (0);
        }
        else
          return;

		m_pImage = m_pAvistream->GetFrame(true);
		if (!m_pAvistream->Eof() && m_pImage ) {
		  //gluScaleImage( GL_RGB, width, height, GL_UNSIGNED_BYTE, m_pImage->Data(0),
		  //               m_iWidth, m_iHeight, GL_UNSIGNED_BYTE, ui_image_copy );

		  // this is about 30-40 times faster then using gluScaleImage(...)
		  // for (int j=0; j<height; ++j)
		  //   memcpy(&m_pCurrentTextureData[j*m_iWidth*3], m_pImage->At(0,j), width*sizeof(uint8_t)*3);

		// avifile < v0.7 swaps top and bottom, lets fix this
	#if AVIFILE_MINOR_VERSION < 7
		  for (int j=0; j<m_iHeight; ++j)
			memcpy(&m_pCurrentTextureData[j*m_iWidth*3], m_pImage->At(0, j), m_iWidth*3);
	#else
		  for (int j=0; j<m_iHeight; ++j)
                  {
                    for (int i=0; i<m_iWidth; ++i)
                    {
                        uint8_t *l_Color = m_pImage->At(i,j);
                        int l_iIndex = (j*m_iWidth+i)*3;
                        m_pCurrentTextureData[l_iIndex] = l_Color[2];
                        m_pCurrentTextureData[l_iIndex+1] = l_Color[1];
                        m_pCurrentTextureData[l_iIndex+2] = l_Color[0];
                    }
                  }
	#endif
		  m_pImage->Release();
		}
	}
}

void	cAVIPlayer::Render(float e_fPosX,float e_fPosY,POINT e_Size)
{
	if( m_bDone||!m_pAvifile )
		return;

	glBindTexture( GL_TEXTURE_2D, m_uiTex_id);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);	// Set Texture Max Filter
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);	// Set Texture Min Filter
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);				// Set The Texture Generation Mode For S To Sphere Mapping
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);				// Set The Texture Generation Mode For T To Sphere Mapping
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_iWidth, m_iHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, m_pCurrentTextureData);

	float	l_UV[] = {0,0,1,1};
	DrawQuadWithTextureAndColorAndCoordinate(e_fPosX,e_fPosY,0.f,e_Size.x,e_Size.y,Vector4::One,l_UV,Vector3::Zero);
}

bool	cAVIPlayer::IsFileOpen()
{
	return m_pAvifile?true:false;
}
#endif






/*

#undef UNICODE
#include<windows.h>
#include<commctrl.h>
#include<vfw.h>

static const char* aviFileName = "movie.avi";
static IAVIFile* aviFile = 0;
static IAVIStream* aviStreamVideo = 0;
static IAVIStream* aviStreamAudio = 0;
static IGetFrame* aviFrame = 0;
static HBITMAP aviFrameBitmap = 0;
static BYTE* aviFrameMemory = 0;
static int aviFrameWidth = 0;
static int aviFrameHeight = 0;
static int aviFramePitchBytes = 0;
static int aviFrameTotalBytes = 0;
static int aviNumberOfFrames = 0;
static int aviCurrentFrame = 0;

// forward declarations
LRESULT CALLBACK MainWndProc(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
bool InitAVIData();
bool FreeAVIData();
bool ExtractAVIFrame(LONG pos);
void FourCCToString(unsigned long code, char* buffer);

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, char*, int)
{
 WNDCLASS wc;
 wc.style         = CS_DBLCLKS;
 wc.lpfnWndProc   = MainWndProc;
 wc.cbClsExtra    = 0;
 wc.cbWndExtra    = 0;
 wc.hInstance     = instance;
 wc.hIcon         = 0;
 wc.hCursor       = LoadCursor(0, IDC_ARROW);
 wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
 wc.lpszMenuName  = 0;
 wc.lpszClassName = "Fundamental Window";

 if(!RegisterClass(&wc))
    return MessageBox(0, "Could not register main window.", "Error", MB_ICONSTOP);

 int x = CW_USEDEFAULT; int w = CW_USEDEFAULT;
 int y = CW_USEDEFAULT; int h = CW_USEDEFAULT;

 HWND window = CreateWindow("Fundamental Window", "Fundamental Window", WS_OVERLAPPEDWINDOW, x, y, w, h, 0, 0, instance, 0);
 if(!window) return MessageBox(0, "Could not create main window.", "Error", MB_ICONSTOP);

 ShowWindow(window, SW_SHOW);
 UpdateWindow(window);

 MSG msg;
 while(GetMessage(&msg, 0, 0, 0)) {
       TranslateMessage(&msg);
       DispatchMessage(&msg);
      }

 return (int)msg.wParam;
}

LRESULT CALLBACK MainWndProc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
 static HWND trackbar = 0;

 switch(message) {
   case(WM_CREATE) : {
        if(!InitAVIData()) return -1;
        trackbar = CreateWindowEx(0, TRACKBAR_CLASS, "", WS_CHILD | WS_VISIBLE | TBS_RIGHT, 0, aviFrameHeight + 5, aviFrameWidth, 25, window, 0, GetModuleHandle(0), 0);
        SendMessage(trackbar, TBM_SETRANGE, (WPARAM)FALSE, (LPARAM)MAKELONG(0, aviNumberOfFrames));
        break;
       }
   case(WM_DESTROY) : {
        DestroyWindow(trackbar);
        FreeAVIData();
        PostQuitMessage(0);
        break;
       }
   case(WM_HSCROLL) : {
        if((HWND)lparam != trackbar) break;
        switch(LOWORD(wparam)) {
          case(TB_THUMBTRACK) : {
               // clamp current frame
               int newpos = (int)SendMessage(trackbar, TBM_GETPOS, 0, 0);
               if(newpos >= aviNumberOfFrames) newpos = aviNumberOfFrames - 1;
               else if(newpos < 0) newpos = 0;
               aviCurrentFrame = newpos;
               SendMessage(trackbar, TBM_SETPOS, (WPARAM)TRUE, (LPARAM)aviCurrentFrame);
               // extract current frame and redraw
               ExtractAVIFrame(aviCurrentFrame);
               InvalidateRect(window, 0, TRUE);
               break;
              }
          case(TB_ENDTRACK) : {
               // clamp current frame
               int newpos = (int)SendMessage(trackbar, TBM_GETPOS, 0, 0);
               if(newpos >= aviNumberOfFrames) newpos = aviNumberOfFrames - 1;
               else if(newpos < 0) newpos = 0;
               aviCurrentFrame = newpos;
               SendMessage(trackbar, TBM_SETPOS, (WPARAM)TRUE, (LPARAM)aviCurrentFrame);
               // extract current frame and redraw
               ExtractAVIFrame(aviCurrentFrame);
               InvalidateRect(window, 0, TRUE);
               break;
              }
         }
        break;
       }
   case(WM_PAINT) : {
        if(!aviFrameBitmap) break;
        PAINTSTRUCT ps;
        HDC winDC = BeginPaint(window, &ps);

        for(int y = 0; y < aviFrameHeight; y++) {
            for(int x = 0; x < aviFrameWidth; x++) {
                WORD* pixel = reinterpret_cast<WORD*>(&aviFrameMemory[aviFramePitchBytes*y + 2*x]);
                BYTE B = (BYTE)(((pixel[0] & 0x001F) << 3));
                BYTE G = (BYTE)(((pixel[0] & 0x03E0) >> 2));
                BYTE R = (BYTE)(((pixel[0] & 0x7C00) >> 7));
                SetPixel(winDC, x, aviFrameHeight - y - 1, RGB(R, G, B));
               }
           }


        //HDC memDC = CreateCompatibleDC(winDC);
        //HBITMAP oldbitmap = (HBITMAP)SelectObject(memDC, aviFrameBitmap);
        //BitBlt(winDC, 0, 0, aviFrameWidth, aviFrameHeight, memDC, 0, 0, SRCCOPY);
        //SelectObject(memDC, oldbitmap);
        //DeleteDC(memDC);
        EndPaint(window, &ps);
        return 0;
       }
  }
 return DefWindowProc(window, message, wparam, lparam);
}

bool InitAVIData()
{
 // initialize VFW
 AVIFileInit();

 // open AVI file
 HRESULT result = AVIFileOpen(&aviFile, aviFileName, OF_READ, 0);
 if(FAILED(result)) return false;

 // extract AVI file information
 AVIFILEINFO fileinfo;
 result = AVIFileInfo(aviFile, &fileinfo, sizeof(fileinfo));
 if(FAILED(result)) { FreeAVIData(); return false; }

 // extract AVI video stream
 result = AVIFileGetStream(aviFile, &aviStreamVideo, streamtypeVIDEO, 0);
 if(FAILED(result)) { FreeAVIData(); return false; }

 // extract AVI audio stream
 result = AVIFileGetStream(aviFile, &aviStreamAudio, streamtypeAUDIO, 0);
 if(FAILED(result)) { FreeAVIData(); return false; }

 // extract AVI video stream information
 AVISTREAMINFO vidsinfo;
 result = AVIStreamInfo(aviStreamVideo, &vidsinfo, sizeof(vidsinfo));
 if(FAILED(result)) { FreeAVIData(); return false; }

 // set bitmap information header
 BITMAPINFOHEADER bminfo;
 bminfo.biSize = sizeof(BITMAPINFOHEADER);
 bminfo.biWidth = vidsinfo.rcFrame.right - vidsinfo.rcFrame.left;
 bminfo.biHeight = vidsinfo.rcFrame.bottom - vidsinfo.rcFrame.top;
 bminfo.biPlanes = 1;
 bminfo.biBitCount = 16;
 bminfo.biCompression = BI_RGB;
 bminfo.biSizeImage = 0;
 bminfo.biXPelsPerMeter = 0;
 bminfo.biYPelsPerMeter = 0;
 bminfo.biClrUsed = 0;
 bminfo.biClrImportant = 0;

 // create AVI video stream frame buffer
 aviFrame = AVIStreamGetFrameOpen(aviStreamVideo, &bminfo);
 if(!aviFrame) { FreeAVIData(); return false; }

 // set AVI frame dimensions
 aviFrameWidth = bminfo.biWidth;
 aviFrameHeight = bminfo.biHeight;
 aviFramePitchBytes = (((16*aviFrameWidth + 31) & (~31))/8);
 aviFrameTotalBytes = (((16*aviFrameWidth + 31) & (~31))/8)*aviFrameHeight;

 // set the number of AVI frames
 aviNumberOfFrames = vidsinfo.dwLength;

 // create dibsection
 BITMAPINFO bi;
 bi.bmiHeader = bminfo;
 HDC winDC = GetDC(0);
 aviFrameBitmap = CreateDIBSection(winDC, &bi, DIB_RGB_COLORS, (LPVOID*)&aviFrameMemory, 0, 0);
 ReleaseDC(0, winDC);
 if(!aviFrameBitmap) { FreeAVIData(); return false; }

 // extract and set current AVI frame
 aviCurrentFrame = 0;
 ExtractAVIFrame(aviCurrentFrame);

 return true;
}

bool FreeAVIData()
{
 // free global variables
 if(aviFrameBitmap) { DeleteObject(aviFrameBitmap); aviFrameBitmap = 0; aviFrameMemory = 0; }
 if(aviFrame) { AVIStreamGetFrameClose(aviFrame); aviFrame = 0; }
 if(aviStreamAudio) { AVIStreamClose(aviStreamAudio); aviStreamAudio = 0; }
 if(aviStreamVideo) { AVIStreamClose(aviStreamVideo); aviStreamVideo = 0; }
 if(aviFile) { AVIFileClose(aviFile); aviFile = 0; }
 aviCurrentFrame = 0;
 aviNumberOfFrames = 0;
 aviFrameWidth = 0;
 aviFrameHeight = 0;
 aviFrameTotalBytes = 0;

 // free VFW
 AVIFileExit();
 return true;
}

bool ExtractAVIFrame(LONG pos)
{
 // must have AVI frame data and dibsection data
 if(!aviFrame) return false;
 if(!aviFrameMemory) return false;

 // extract the frame
 LPVOID bmdata = AVIStreamGetFrame(aviFrame, pos);
 if(!bmdata) return false;

 // copy extracted frame data into dibsection memory
 int offset = sizeof(BITMAPINFOHEADER); // no color table for 16 or 24-bit bitmaps
 memmove((char*)aviFrameMemory, (char*)bmdata + offset, aviFrameTotalBytes);

 return true;
}

void FourCCToString(unsigned long code, char* buffer)
{
 buffer[0] = (char)((code)       & 0xff);
 buffer[1] = (char)((code >>  8) & 0xff);
 buffer[2] = (char)((code >> 16) & 0xff);
 buffer[3] = (char)((code >> 24) & 0xff);
 buffer[4] = '\0';
}


*/

/*

//Example of IAviReadFile & IAviReadStream interface usage.
//Copyright 2000 Eugene Kuznetsov (divx@euro.ru)

#include <avm_default.h>
#include <avifile.h>
#include <aviplay.h>
#include <avm_except.h>
#include <version.h>

#include <stdio.h>
#include <stdlib.h> // exit

#define __MODULE__ "extractor"

int main(int argc, char** argv)
{
    static const int buffer_size = 20000;
    static const char* outname = "./extractor.mp3";
    FILE* f = 0;
    avm::IReadFile* ac = 0;
    avm::IReadStream* as = 0;
    uint8_t* zz = 0;
    if (GetAvifileVersion() != AVIFILE_VERSION)
    {
	fprintf(stderr, "This binary was compiled for Avifile ver. %d "
		", but the library is ver. %d. Aborting.\n",
		AVIFILE_VERSION, GetAvifileVersion());
	return 0;
    }

    if (argc == 0)
    {
	fprintf(stderr, "Missing argument: filename\n");
	exit(0);
    }

    try
    {
	if (!(f = MyFileOpen(outname, "wb")))
	    throw FATAL("Can't open %s for writing", outname);
	if (!(ac = avm::CreateReadFile(argv[1])))
	    throw FATAL("Can't read given file");
	if (!(as = ac->GetStream(0, avm::IStream::Audio)))
	    throw FATAL("Stream doesn't contain audio stream");

	zz = new uint8_t[buffer_size];
	while (!as->Eof())
	{
	    uint_t samp_read, bytes_read;
	    as->ReadDirect(zz, buffer_size, buffer_size, samp_read, bytes_read);
	    fwrite(zz, bytes_read, 1, f);
	}
    }
    catch (FatalError& error)
    {
	error.Print();
    }

    delete ac;
    delete zz;
    if (f)
	fclose(f);
}
*/

//case 20000:
//{
////MessageBox(hwnd," Open AVI ","Information",MB_OK | MB_ICONINFORMATION);
//
//AVIFileOpen(&pAviIn, "C:\\...\\Test.avi", OF_READ, nullptr);
//
//rc = AVIFileInfo(pAviIn, &pFileInfo, sizeof(AVIFILEINFO));
//int i;
//for (i = 0; i < pFileInfo.dwStreams; i++) 
//{ 
//rc = AVIFileGetStream(pAviIn, &pAviStream, 0L, i);
//if (rc) break;
//
//rc = AVIStreamInfo(pAviStream, &StreamInfo, sizeof(AVISTREAMINFO)); 
//if (StreamInfo.fccType == streamtypeVIDEO)
//pVideoStream = pAviStream;
//else if (StreamInfo.fccType == streamtypeAUDIO)
//pAudioStream = pAviStream;
//}
////if(pVideoStream != 0) MessageBox(hwnd," AVI opened. ","Information",MB_OK | MB_ICONINFORMATION);
//bFileOpen = TRUE;
//
//pVideoFrame = AVIStreamGetFrameOpen(pVideoStream, (LPBITMAPINFOHEADER)AVIGETFRAMEF_BESTDISPLAYFMT);
//
//hDrawDibDC = DrawDibOpen();
//
//InvalidateRect(hwnd,0,TRUE);
//}
//
//break;
//
//case 20002:
//{
//LONG nAviStreams = 0L; // number of pAviStreams[];
//PAVISTREAM pStreams[2] = { nullptr, nullptr };
//PAVISTREAM pAviTmpStream = nullptr;
//
//AVICOMPRESSOPTIONS Options[2];
//LPAVICOMPRESSOPTIONS pOptions[2];
//
//AVIFILEINFO pFileInfo;
//AVISTREAMINFO pStreamInfo; 
//
//AVIFILEINFO pFileInfo2;
//AVISTREAMINFO pStreamInfo2;
//
//
////AVIFileOpen(&pAviOut, "AviCopy.avi", OF_CREATE|OF_WRITE, nullptr);
////pStreams[0] = pVideoStream;
//
//LPVOID frame_format;
//LONG frame_format_size;
//char msg2[50] = "";
//
//AVIStreamReadFormat(pVideoStream, 0, 0, &frame_format_size);
//sprintf(msg2,"FormatSize: %ld ",frame_format_size); 
//MessageBox(0,msg2,"",MB_OK);
//frame_format = malloc(frame_format_size);
//AVIStreamReadFormat(pVideoStream, 0, frame_format, &frame_format_size);
//
//AVIFileOpen(&pAviOut, "AviStreamCreate.avi", OF_CREATE|OF_WRITE, nullptr);
//AVIStreamInfo(pVideoStream, &pStreamInfo, sizeof(AVISTREAMINFO));
//sprintf(msg2,"InfoTest: %d ",pStreamInfo.rcFrame.right ); 
//MessageBox(0,msg2,"",MB_OK);
//
//AVIFileCreateStream(pAviOut, &pAviTmpStream, &pStreamInfo);
//AVIStreamSetFormat(pAviTmpStream, 0, frame_format, frame_format_size);
//
//LPVOID frame = 0;
//LONG frame_size;
//int n = 0;
//while(AVIStreamRead(pVideoStream, n, 1, nullptr, 0, &frame_size, nullptr) == 0 && n < 10)
//{
//frame = malloc(frame_size);
////sprintf(msg2,"FrameSize: %ld ",frame_size); 
////MessageBox(0,msg2,"",MB_OK);
//AVIStreamRead(pVideoStream, n, 1, frame, frame_size, nullptr, nullptr);
//if(AVIStreamIsKeyFrame(pAviStream, n))
//{
//AVIStreamWrite(pAviTmpStream, n, 1, frame, frame_size, AVIIF_KEYFRAME, nullptr, nullptr);
////MessageBox(0,"Key","",MB_OK);
//}
//else
//{
//AVIStreamWrite(pAviTmpStream, n, 1, frame, frame_size, 0, nullptr, nullptr);
//}
//free(frame);
//frame = 0;
//n++;
//sprintf(msg2,"Frame: %ld ",n); 
//SetWindowText(hwnd,msg2);
//}
//sprintf(msg2,"Anzahl Frames: %d ",n); 
//MessageBox(0,msg2,"",MB_OK);
//free(frame_format);
//AVIStreamRelease(pAviTmpStream);
///*// get save (compression) options
//int i;
//for (i = 0; i < 1; i++)
//{
//// AVISaveOptions takes an array of pointers to our 
//// compression options (cleared)
//pOptions[i] = &Options[i];
//memset(pOptions[i], 0, sizeof(AVICOMPRESSOPTIONS));
//}
//
//AVISaveOptions(hwnd, 0, 1,pStreams,pOptions);
//
//AVISave("AviEdit.avi", nullptr, SaveCallback, 1,pStreams[0], pOptions[0]);
//AVISaveOptionsFree(1, pOptions);*/
//
//AVIFileRelease(pAviOut);
//
//}
//
//break;


//Call AVIFileInit.
//Call AVIFileOpen to create a new file (returns PAVFILE*).
//Call AVIFileCreateStream to make a new stream (requires an AVISTREAMINFO struct to describe the stream).
//Call AVIStreamSetFormat to write a format header to match whatever compression is being used.
//Call AVIStreamWrite for each frame.
//Call (in order) AVIStreamRelease, AVIFileRelease, AVIFileExit.


//end WIN32
#endif