#ifndef	_DOWNLOAD_FILE_H_
#define	_DOWNLOAD_FILE_H_

	class	cDownloadFile
	{
		int				m_iProgress;
	public:
		cDownloadFile();
		~cDownloadFile();
		bool			StartToDownload(const char*e_strDownloadFileURL,const char*e_strDescription,const char* e_strTitle,const char*e_strDownloadFileName);
		void			Update(float e_fElpaseTime);
		void			Render();
		//-1 download failed,100 done
		int				GetProgress(){return m_iProgress;}
		std::string		m_strDownloadFileName;
	};

#endif