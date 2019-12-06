#ifndef FFMPEG_PLAYER_H
#define FFMPEG_PLAYER_H

#ifdef WIN32
#include"AndroidVideoPlayer.h"
#elif defined(ANDROID)

//in the Android side
//setting the Linker -> Advanced -> Report Undefined Symbols to No
//- this property controls the same linker flag as the LOCAL_ALLOW_UNDEFINED_SYMBOLS 
//in ndk-build. Hope that helps!

//if windows show cannot find entry point in glewInit.dll   xxxxx avdecode.dll
//
//go to project->properties->Linker->Optmize->set first and second as default
//
//
#include "../Image/Texture.h"
#include "../Sound/OpenAlWavFile.h"

struct AVFormatContext;
struct AVStream;
struct AVPacketList;
struct AVFrame;
struct AVPacket;
struct AVCodecContext;
struct SwsContext;

#define NUM_AUDIO_BUFFERS 3
#define AUDIO_BUFFER_SIZE 4096*4
class	VideoState;

class	cVideoPlayer:public NamedTypedObject
{
	cFUThreadPool	m_FUThreadPool;
	VideoState*		m_pVideoState;
	AVPacket*       m_pflush_pkt;
	//
	void			DestroyBySingleThread();
	void			DestroyByMultithreadThread();
protected:
	void			SaveFrame(AVFrame *pFrame, int width, int height, int iFrame);
	float			GetFPS(AVCodecContext*e_pAVCodecContext);
	//below is single thread
	//=================================================
	bool			m_bPlayOver;

	struct	sAudioData
	{
		int				iStreamIndex;
		AVStream*		pStream;
		AVCodecContext* pCodecContext;
		AVFrame*		pFrame;
		ALuint			uiSource,uiBuffers[NUM_AUDIO_BUFFERS];	
		int				iFrequency;
		ALenum			iFormat;
		bool			bInit;
		int				iBufferNumber;
		sAudioData();
		~sAudioData();
	};
	struct	sVideoData
	{
		AVFrame*		pFrame;
		AVFrame*		pRGBFrame;//store pixelx data
		AVCodecContext*	pCodecContext;
		SwsContext*		pImageConveteContext;
		int             iStreamIndex;
		char*			pRGBFrameBuffer;
		cBaseImage		*pTexture;
		sVideoData();
		~sVideoData();
	};
	sVideoData*						m_pVideoData;
	sAudioData*						m_pAudioData;
	AVFormatContext*				m_pFormatCtx;
	float							m_fFPS;
	float							m_fVideoLength;
	//current play progess
	float							m_fCurrentime;
	//is it time to render
	float							m_fCurrentTimeToDetectedTimeForRender;
	bool							m_bRender;
	//===============================================
	bool							AssignVideoData();
	bool							AssignAudioData();
	void							UpdateBySingleThread(float e_fElpaseTime);
public:
	cVideoPlayer();
	virtual ~cVideoPlayer();
	bool	OpenFileBySingleThread(char*e_strFileName);
	void	Destroy();
	//below is multithreads
	bool	OpenFile(const char*e_strFileName);
	//for multi thread
	float	GetDuration();
	float	GetCurrenTime(){ return this->m_fCurrentime; }
	void	SetCurrentTime(float e_fCurrentime);
	void    Update(float e_fElpaseTime);
	void    Render();
	bool	IsPlayDone();
};
//end WIN32
#endif
//end FFMPEG_PLAYER_H
#endif