#ifndef	_FB_FUNCTION_H_
#define	_FB_FUNCTION_H_

class	cFBFunction
{
	cPhasePopUpMessager*	m_pFBSharePrizeConfirmPopUpMessager;
	static	void			WriteTimeStamp(char*e_strFileName,int e_iMagicID);
	static	float			GetTimeDiffByTimeStampFile(char*e_strFileName,int e_iMagicID);
public:
	cFBFunction();
	~cFBFunction();
	void	Update(float e_fElpaseTime);
	static	void	DoFBSharePrize();
	static	void	DoFBLinkInfo();
};

#endif