#ifndef _TITLE_PHASE_H_
#define _TITLE_PHASE_H_


class	cTitlePhase:public ISAXCallback,public cSimplePhase
{
	std::string										m_strTutorialVideoLink;
	std::string										m_strFansLink;
	cMPDI*											m_pOpeningMPDI;
	cClickMouseBehaviorVector<cImageButton>*		m_pClickMouseBehaviorVector;
	virtual	void	HandleElementData(TiXmlElement*e_pTiXmlElement);
public:
	cTitlePhase();
	~cTitlePhase();
	virtual	void			Init();
	virtual	void			Destroy();
	virtual	void			Update(float e_fElpaseTime);
	virtual	void			Render();
	virtual	void			DebugRender();
	virtual	void			MouseDown(int e_iX,int e_iY);
	virtual	void			MouseUp(int e_iX,int e_iY);
	virtual	void			MouseMove(int e_iX,int e_iY);
};

#endif