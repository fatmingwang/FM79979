#ifndef _SETUP_PHASE_H_
#define _SETUP_PHASE_H_

class	cSetupPhase:public cSimplePhase,public ISAXCallback
{
	cMPDI*					m_pBGMPDI;
	cBasicSound*			m_pSoundEffect;
	cBasicSound*			m_pSoundBGM;
	cBinaryFile				m_SoundVolumeFile;
	cResourceStamp			m_ResourceStamp;
	//
	cSliderBar*				m_pEffectVolumeSlider;
	cSliderBar*				m_pBGMVolumeSlider;
	cImageButton*			m_pApplyButton;
	cImageButton*			m_pChancelButton;
	cImageButton*			m_pMuteButton;
	cClickMouseBehaviorVector<cImageButton>	m_AllButton;
	float					m_fOriginalEffectVolume;
	float					m_fOriginalBGMVolume;
	//
	virtual	void			HandleElementData(TiXmlElement*e_pTiXmlElement);
public:
	cSetupPhase();
	~cSetupPhase();
	virtual	void	Destroy();
	virtual	void	Update(float e_fElpaseTime);
	virtual	void	Init();							//it would be called if the stage is changed.
	virtual	void	Render();
	virtual	void	DebugRender();
	virtual	void	MouseDown(int e_iX,int e_iY);
	virtual	void	MouseUp(int e_iX,int e_iY);
	virtual	void	MouseMove(int e_iX,int e_iY);
	virtual	const   WCHAR*	GetNextPhaseName();
	virtual	void	FetchData(const WCHAR*e_strPhaseName,void*e_pData);
	std::wstring	m_strLastPhaseName;
};

#endif