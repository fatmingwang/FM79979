#ifndef	_UNPACK_EXPANSION_PACK_H_
#define	_UNPACK_EXPANSION_PACK_H_

	class	cUnZip;
	class	cUnpackExpansionPack
	{
		bool			m_bCheckedFileExists;
		std::string		m_strUnzipFileName;
		std::string		m_strCheckedFileName;
		cUnZip*			m_pUnZip;
		bool			IsUnpacked();
		void			DoUnpack();
	public:
		cUnpackExpansionPack();
		~cUnpackExpansionPack();

		bool	IsInUnpackProgress();
		void	Update(float e_fElpaseTime);
		void	Render();
	};

#endif