#ifdef WIN32
#pragma once
#ifdef WIN32
//#pragma comment(lib, "Vfw32.lib")
#include "../../../Common/CommonDefine.h"
#include "vfw.h"
#include "../../OpenGL/Glh.h"
#else
#include "../../../Common/CommonDefine.h"
// OpenGL specific
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

// Video specific

#include <avifile/avifile.h>
#include <avifile/version.h>

#if AVIFILE_MINOR_VERSION > 6
  #include <avifile/avm_default.h>
#else
  #include <avifile/default.h>
#endif

#include <avifile/image.h>
#include <avifile/StreamInfo.h>


#include <fstream>
#include <iostream>
#include <sys/types.h>
#endif
namespace	FATMING_CORE
{
	class	cBasicSound;
}

#define AUDIO_BUFFER_SIZE 1024
#define MAX_AUDIOQ_SIZE (5 * 16 * 1024)
#define MAX_VIDEOQ_SIZE (5 * 256 * 1024)

class	cAVIPlayer
{
	FATMING_CORE::cBasicSound*		m_pBasicSound;
	int								m_iCurrentAudioStep;
	int								m_iAudioStreamLength;
	int								m_PerSampleBit;
	void							SetAudioPosByTime(float e_fTime);
#ifdef WIN32
	IAVIFile*			m_pAviFile;									// AVI file handle.
	PAVISTREAM			m_pAVIStreamVideo;							// Handle To An Open Stream,for video.
	PAVISTREAM			m_pAVIStreamSound;							// Handle To An Open Stream,for sound.
	AVISTREAMINFO		m_PSI;										// Pointer To A Structure Containing Stream Info.
	AVISTREAMINFO		m_PSIAudio;									// Pointer To A Structure Containing Stream Info.
	unsigned char*		m_pDIBData;									// Pointer To Our Resized Image.
	HDC					m_hdc;										// Creates A Compatible Device Context.
	HBITMAP				m_hBitmap;									// Handle To A Device Dependant Bitmap.
	PGETFRAME			m_PGF;										// Pointer To A GetFrame Object.
	BITMAPINFOHEADER	m_Bmih;										// Header Information For DrawDibDraw Decoding.
	HDRAWDIB			m_HDD;										// Handle For Our Dib.
	void				GrabAVIFrame(int frame);					// Grabs A Frame From The Stream.
	void				ParseVideoStream();
	void				ParseAudioStream();
	void				UpdateVideo(float e_fElpaseTime);
	void				UpdateSound(float e_fElpaseTime);
#else
	avm::IReadFile*		m_pAvifile;									// represents the avi file
	avm::IReadStream*	m_pAvistream;								// represents the avi video stream
	avm::IReadStream*	m_pAudiostream;								// represents the avi audio stream
	StreamInfo*			m_pStreaminfo;								// represents the avi streaminfo
	CImage*				m_pImage;									// an image (provided by avifile)
#endif
	int					m_iWidth;									// Video Width.
	int					m_iHeight;									// Video Height.
	long				m_lLastframe;								// Last Frame Of The Stream.
	int					m_iMPF;										// Will Hold Rough Milliseconds Per Frame.
	char*				m_pCurrentTextureData;						// Pointer To Texture Data(while it's on womdows it could aviod screen refersh unnormally for m_pDIBData).
	int					m_iNext;									// Used For Animation.
	int					m_iPrior;									// Used For Animation.
	int					m_iFrame;									// Frame Counter.
	bool				m_bDone;									// play over?
	bool				m_bLoop;									// loopable.
	GLuint				m_uiTex_id;									//texture id.
	double				m_dbTimeLength;								//time length.
	float				m_fFPS;										//frame per-second
	bool				m_bPause;									//paus
public:
	cAVIPlayer();
	virtual ~cAVIPlayer();
	void	CloseFile();
	//if the file is opened call close before recall openfile
	bool	OpenFile(const wchar_t*e_strFileName);
	void	Update(float e_fElpaseTime);
	void	RenderByTime(float e_fTime,float e_fPosX,float e_fPosY,POINT e_Size);
	void	Render(float e_fPosX,float e_fPosY,POINT e_Size);
	void	SetLoop(bool e_bLoop){ m_bLoop = e_bLoop; }
	bool	IsFileOpen();
	long	GetLastframe(){ return m_lLastframe; }
	int     GetCurrentFrame(){return m_iFrame;}
	void	SetCurrentFrame(int e_iFrame);
	void	SetCurrentFrameByTime(float e_fTime);
	double	GetFileTimeLength();
	float	GetFPS(){ return m_fFPS; }
	void	SetPause(bool e_b){ m_bPause = e_b; }
	bool	GetPause(){ return m_bPause; }
	int		GetWidth(){return m_iWidth;}
	int		GetHeight(){return m_iHeight;}
};
//end WIN32
#endif