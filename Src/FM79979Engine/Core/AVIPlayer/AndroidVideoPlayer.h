#ifdef USE_OPEN_CV

#ifndef	_ANDROID_VIDEO_PLAYER_H_
#define	_ANDROID_VIDEO_PLAYER_H_

	//from windows opencv
	//#pragma comment(lib, "../../../lib/opencv_highgui249.lib")
	//#pragma comment(lib, "../../..//lib/opencv_core249.lib")
	//#pragma comment(lib, "../../..//lib/opencv_imgproc249.lib")
	struct CvCapture;
	//struct IplImage;
	namespace FATMING_CORE
	{
		class	cBaseImage;
		class	cBasicSound;
	}
	class	cVideoPlayer:public NamedTypedObject
	{
	protected:
		int			m_iVideoWidth;
		int			m_iVideoHeight;
		//
		FATMING_CORE::cBaseImage*	m_pVideoImage;
		CvCapture*	m_pCvCapture;
		float		m_fFPS;
		int			m_iTotalFrame;
		float		m_fRestTimeToNextFrame;
		float		m_fTotalPlayTime;
		float		m_fCurrentPlayTime;
		int			GetCurrentFrame(float e_fTime);
	public:
		cVideoPlayer();
		virtual ~cVideoPlayer();
		//below is multithreads
		virtual	bool	OpenFile(const char*e_strFileName);
		virtual	void    Update(float e_fElpaseTime);
		virtual	void    Render();
		virtual	bool	IsPlayDone();
	};

	class	cVideoConvert:public cVideoPlayer
	{
	protected:
		//temp to store jpeg data
		int					m_iMaximumTampFileLength;
		char*				m_pTempData;
		//
		bool				SaveFrameToFile(int e_iFrame,void*e_pIplImage,NvFile*e_pFile);
		//
		UINT				m_uiCurrentFilePos;
		int					m_iCurrentFrame;
		UINT*				m_puiFrameFilePosVector;
		UINT*				m_puiFrameSizeVector;
		//
		bool				FirstConvertToFile(NvFile*e_pFile);
		bool				ConvertToFile(int e_iFrame,NvFile*e_pFile);		//done return 1
		float				PlayCompress(int e_iFrame,NvFile*e_pFile,int e_iFrameSize);	
		UINT				GetFrameSize(UINT e_iFrame);
	public:
		cVideoConvert();
		virtual ~cVideoConvert();
		bool	StartConvert(NvFile*e_pFile);
		float	GetProgress();
		int		m_iTargetWidth;
		int		m_iTargetHeight;
		int		m_iQuality;
	};

	class	cFMVVideo:public cVideoConvert
	{
		cBasicSound*				m_pVideoSound;
		//
		bool						m_bDoPreparePowerOfTwoTexture;
		UT::sTimeAndFPS				m_FPS;
		int							m_iVideoChannal;
		char**						m_pNewPixelData;
		char**						m_pNewPowerOfTwoPixelData;
		int							m_iNumPacket;
		cFUSynchronized				m_FUSynchronized;
		cFUThreadPool*				m_pFUThreadPool;
		NvFile*						m_pFile;
		bool						m_bForceDone;
		int							m_iCurrentUpdatePixelIndex;
		std::list<char*>			m_WaitForProcessData;
		void						DecodeFrame(int e_iFrameIndex);
		bool						m_bLoop;
	public:
		cFMVVideo();
		virtual ~cFMVVideo();
		virtual	bool				OpenFile(const char*e_strFileName);
		virtual	void				Update(float e_fElpaseTime);
		virtual	void				Render();
		virtual	bool				IsPlayDone();
		bool						IsForceDone(){return m_bForceDone;}
		bool						m_bUsingMultiThread;
		bool						m_bUsingSoundThread;
		const char*					GetFPS(){return m_FPS.GetFPS();}
		void						Destroy();
		bool						IsLoop(){ return m_bLoop; }
		void						SetLoop(bool e_bLoop){ m_bLoop = e_bLoop; }
		void						SetTime(float e_fTime);
		float						GetcurrentTime();
		int							GetCurrentFrame();
		cBasicSound*				GetVideoSound(){return m_pVideoSound;}
		//delete it after use
		cBaseImage*					GetLastFrameImage();
	};
//end	_ANDROID_VIDEO_PLAYER_H_
#endif
//USE_OPEN_CV
#endif