#ifndef	_ANDROID_VIDEO_PLAYER_H_
#define	_ANDROID_VIDEO_PLAYER_H_

	struct CvCapture;
	//struct IplImage;
	namespace FATMING_CORE
	{
		class	cBaseImage;
	}
	class	cVideoPlayer
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
		~cVideoConvert();
		bool	StartConvert(NvFile*e_pFile);
		float	GetProgress();
	};

	class	cFMVVideo:public cVideoConvert
	{
		cFUSynchronized				m_FUSynchronizedForFileRead;
		//just a temp data to store sequence pixel data by frame index
		char**						m_ppTempPixelDataForSync;
		bool*						m_pbWaitSingnalForThred;
		int							m_iThreadCount;
		char**						m_pMultiThreadReadFileTempData;
		friend void					VideoThread(size_t _workParameter, size_t _pUri);
		//
		bool						m_bDoPreparePowerOfTwoTexture;
		UT::sTimeAndFPS				m_FPS;
		int							m_iVideoChannal;
		char**						m_pNewPixelData;
		char**						m_pNewPowerOfTwoPixelData;
		int							m_iNumPacket;
		cFUSynchronized				m_FUSynchronized;
		cFUThreadPool				m_FUThreadPool;
		NvFile*						m_pFile;
		bool						m_bForceDone;
		int							m_iCurrentUpdatePixelIndex;
		std::list<char*>			m_WaitForProcessData;
		void						DecodeFrame(int e_iFrameIndex);
		void						MultiThreadDecodeFrame(int e_iFrameIndex,int e_iThreadIndex);
	public:
		cFMVVideo();
		~cFMVVideo();
		virtual	bool				OpenFile(const char*e_strFileName);
		virtual	void				Update(float e_fElpaseTime);
		virtual	void				Render();
		virtual	bool				IsPlayDone();
		bool						IsForceDone(){return m_bForceDone;}
		bool						m_bUsingMultiThread;
		const char*					GetFPS(){return m_FPS.GetFPS();}
		int							m_iWaitCount;
		void						Destroy();
	};
//end	_ANDROID_VIDEO_PLAYER_H_
#endif