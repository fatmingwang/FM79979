#ifndef	_DOWNLOAD_FILE_PHASE_H_
#define	_DOWNLOAD_FILE_PHASE_H_

	class	cUnZip;
	class	cDownloadFile;
	class	cDownloadContentPhase:public cSimplePhase
	{
		enum	eStatus
		{
			eS_WAITNING_DOWNLOAD = 0,
			eS_DOWNLOAD_FAILED,
			eS_UNZIPPING,
			eS_UNZIP_FAILED,
			eS_SHOW_ALL_DONE_MESSAGE,
		};
		eStatus			m_eStatus;
		cUnZip*			m_pUnZip;
		cDownloadFile*	m_pDownloadFile;
		cTextButton*	m_pTextButton;
		std::string		m_strExtractDirectory;
		std::string		m_strZipPassword;
		std::wstring	m_strDownloadFontName;
		cPhasePopUpMessager*	m_pPhasePopUpMessager;
	public:
		cDownloadContentPhase(TiXmlElement*e_pPhasePopUpMessagerElement);
		~cDownloadContentPhase();
		bool	StartToDownload(const char*e_strDownloadFileURL,const char*e_strDescription,const char* e_strTitle,const char*e_strDownloadFileName,const char*e_strZipPassword,const char*e_strExtractDirectory,bool e_bDeleteFileAfterExtract);
		virtual	void	Init(){}
		virtual	void	Update(float e_fElpaseTime);
		virtual	void	Render();
        virtual void    MouseDown(int e_iPosX,int e_iPosY);
        virtual void    MouseMove(int e_iPosX,int e_iPosY);
        virtual void    MouseUp(int e_iPosX,int e_iPosY);
		bool			IsDownloadComplete();
	};

#endif