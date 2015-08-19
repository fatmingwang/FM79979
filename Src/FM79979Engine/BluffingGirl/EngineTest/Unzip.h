#ifndef	_UNZIP_H_
#define	_UNZIP_H_

	class	cUnZip
	{
		int		m_iProgress;
		void	CheckProgress();
	public:
		cUnZip();
		~cUnZip();
		bool	StartUnzipInDownloadFolder(const char*e_strFileName,const char*e_strExtractDirectory,const char*e_strPassword = "79979");
		bool	StartUnzipInSystemFolder(const char*e_strFileName,const char*e_strExtractDirectory,const char*e_strPassword = "79979");
		void	Update(float e_fElpaseTime);
		void	Render();
		//-1 for unzip failed,100 for unzip done
		int		GetProgress(){return m_iProgress;}
	};

#endif