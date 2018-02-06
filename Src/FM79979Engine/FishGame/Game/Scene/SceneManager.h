#pragma once

class cBaseScene;
class cCameraShake;

class cSceneChange : public ISAXCallback,public cPhaseManager
{
private:
	cCameraShake * m_pCameraShake;
	virtual	void	HandleElementData(TiXmlElement*e_pTiXmlElement);
	void			ProcessBG(TiXmlElement*e_pTiXmlElement);

public:	cSceneChange();
	~cSceneChange();
	void	LastRender();
	void	DebugRender();
	void	Update(float e_fElpaseTime);
};