#ifndef _VIDEO_SUB_MPDI_H_
#define _VIDEO_SUB_MPDI_H_

#ifdef WIN32
#include "SubMPDI.h"

class	cAVIPlayer;

namespace FATMING_CORE
{
	class	cVideoSubMPDI:public cSubMPDI
	{
		cAVIPlayer*m_pAVIPlayer;
		virtual		void	InternalInit();
		virtual		void	InternalUpdate(float e_fElpaseTime);
		virtual		void	InternalRender();
		float				m_fFilmStartTime;
		float				m_fFilmEndTime;
	public:
		cVideoSubMPDI();
		virtual ~cVideoSubMPDI();
		bool			OpenFile(const char*e_strFileName);
		cAVIPlayer*		GetAVIPlayer(){return m_pAVIPlayer;}
		virtual void    RenderFirsttFrameImage(bool e_bBlending,Vector4 e_vColor);
		virtual	void	RenderByGlobalTime(float e_fTime);
		virtual	void	RenderLastFrame();
	};
}
//end win32
#endif
//end _VIDEO_SUB_MPDI_H_
#endif