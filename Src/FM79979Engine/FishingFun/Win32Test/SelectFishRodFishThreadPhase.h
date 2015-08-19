#ifndef	_FISH_ROD_FISH_THREAD_SELECT_PHASE_H_
#define	_FISH_ROD_FISH_THREAD_SELECT_PHASE_H_

#include "FishRodAndFishThreadData.h"

class	cGameObjectSelector;
class	cFishingPhase;
class	cSelectFishRodFishThreadPhase:public ISAXCallback,public cSimplePhase
{
	friend class								cFishingPhase;
	//
	std::wstring								m_strStageName;
	cNamedTypedObjectVector<NamedTypedObject>	m_SelectableFishThread;
	//
	cMPDI*										m_pUIBGMPDI;
	cGameObjectSelector*						m_pFishRodGameObjectSelector;
	cGameObjectSelector*						m_pFishThreadGameObjectSelector;
	cButtonDialog*								m_pYesNoButton;
	//
	virtual	void								HandleElementData(TiXmlElement*e_pTiXmlElement);
public:
	cSelectFishRodFishThreadPhase();
	~cSelectFishRodFishThreadPhase();
	virtual	void	Init();//it would be called if the stage is changed,set m_bSatisfiedCondition as false!?
	virtual	void	Update(float e_fElpaseTime);
	virtual	void	Render();
	virtual	void	Destroy();
	virtual	void	DebugRender();
	virtual	void	MouseDown(int e_iPosX,int e_iPosY);
	virtual	void	MouseMove(int e_iPosX,int e_iPosY);
	virtual	void	MouseUp(int e_iPosX,int e_iPosY);
	virtual	void*	GetData();
	virtual	void	FetchData(const WCHAR*e_strPhaseName,void*e_pData);
};


#endif