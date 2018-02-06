#pragma once

class cBaseScene:public cSimplePhase
{
	cMPDI*m_pBG;
	//UT::sMinMaxData<float>	 m_TimeCircleRestrict;
	//UT::sTimeCounter		 m_ChangeSceneCircle;
public:
	static bool	m_bGenerateMonster;
	cBaseScene(TiXmlElement*e_pTiXmlElement);
	~cBaseScene();
	virtual	void	Init() override;
	virtual	void	Update(float e_fElpaseTime) override;
	virtual	void	Render() override;
};


class cBossScene :public cBaseScene
{
public:
	cBossScene(TiXmlElement*e_pTiXmlElement);
	~cBossScene();
	virtual	void	Init() override;
	virtual	void	Update(float e_fElpaseTime) override;
	virtual	void	Render() override;
};