#pragma once

class cBaseScene:public cSimplePhase, public cMessageSender
{
	enum eSceneStatus
	{
		eSS_START = 0,
		eSS_LOOP,
		eSS_END,
		eSS_MAX
	};
	cMPDI*m_pBGMPDI[eSS_MAX];
	eSceneStatus	m_eCurrentSceneStatus;
	//
	std::wstring*		m_pstrSceneChangeFishGroupName;
	UT::sTimeCounter*	m_pToNextSceneTC;
	//
	void				UpdateStartStatus(float e_fElpaseTime);
	void				UpdateLoopStatus(float e_fElpaseTime);
	void				UpdateEndStatus(float e_fElpaseTime);
	//
	void				ToNextScene();
	//
	bool				FishGroupFinishEvent(void*e_pFishGroupName);
public:
	DEFINE_TYPE_INFO();
	cBaseScene(TiXmlElement*e_pTiXmlElement);
	virtual ~cBaseScene();
	virtual	void	Init() override;
	virtual	void	Update(float e_fElpaseTime)override;
	virtual	void	Render() override;
	virtual	void	DebugRender() override;
};


//class cBossScene :public cBaseScene
//{
//public:
//	cBossScene(TiXmlElement*e_pTiXmlElement);
//	~cBossScene();
//	virtual	void	Init() override;
//	virtual	void	Update(float e_fElpaseTime) override;
//	virtual	void	Render() override;
//};