#ifndef _MAGIC_TOWER_UI_INFO_H_
#define _MAGIC_TOWER_UI_INFO_H_

	class	cUIInfo
	{
		cMPDI*		m_pUIInfoMPDI;
	public:
		cUIInfo();
		~cUIInfo();
		void	Update(float e_fElpaseTime);
		void	Render();
		cMPDI*	GetUIInfoMPDI(){return m_pUIInfoMPDI;}
	};


#endif