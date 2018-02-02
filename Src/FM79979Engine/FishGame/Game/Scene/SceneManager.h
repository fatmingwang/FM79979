#pragma once


enum eSceneChangeStatus
{
	eSCS_NONE = 0,
	eSCS_PLAY_SCENE_CHANGE_MPDI,
	eSCS_WAIT_FISH_LEAVE,
	eSCS_BUBBLE_GO,
	eSCS_WAIT_BUBBLE_DISSAPEAR,
	eSCS_BONUS_ALERT_GO,
	eSCS_WAIT_BONUS_ALERT_DISSAPEAR,
	eSCS_MAX,
};

//shake
//float duration;
//float speed;
//float magnitude;
//float elapsed;
//float randomStart;

class cSceneChange : public ISAXCallback
{		
private:
	//UT::sMinMaxData<float>	 m_TimeCircleRestrict;
	//UT::sTimeCounter		 m_ChangeSceneCircle;
	void Shake(float e_fElpaseTime);
	virtual	void	HandleElementData(TiXmlElement*e_pTiXmlElement);
	void			ProcessBG(TiXmlElement*e_pTiXmlElement);
	void			ProcessSpecialBG(TiXmlElement*e_pTiXmlElement);
	void			ProcessBonusBG(TiXmlElement*e_pTiXmlElement);
	void			ProcessNormalBG(TiXmlElement*e_pTiXmlElement);
	void			ProcessSpecialScene(TiXmlElement*e_pTiXmlElement);
	void			ProcessBonusScene(TiXmlElement*e_pTiXmlElement);
	void			ProcessLittlePrize(TiXmlElement*e_pTiXmlElement);
	//
	void			ChangeScene();
	void			ChangeSpecialScene();
	void			ChangeBonusScene();
	void			UpdateeSCS_NONE(float e_fElpaseTime);
	void			UpdateeeSCS_PLAY_SCENE_CHANGE_MPDI(float e_fElpaseTime);
	void			UpdateeeSCS_WAIT_FISH_LEAVE(float e_fElpaseTime);
	void			UpdateeeSCS_BUBBLE_GO(float e_fElpaseTime);
	void			UpdateeeSCS_WAIT_BUBBLE_DISSAPEAR(float e_fElpaseTime);
	void			UpdateeeSCS_BONUS_ALERT_GO(float e_fElpaseTime);
	void			UpdateeeSCS_WAIT_BONUS_ALERT_DISSAPEAR(float e_fElpaseTime);
	cMatrix44		m_matDragonBallTransform;
	bool			IsAllowToChangeScene();

public:	cSceneChange();
	~cSceneChange();
	void	Init();
	void	Render();
	void	LastRender();
	void	DebugRender();
	void	Update(float e_fElpaseTime);
	bool	IsChangeScene();
};