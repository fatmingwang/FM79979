#pragma once

//a time with curve  show each key points as hint(samlle to big with alpha change)
//
//setp 1.show 3,2,1
//step 2.show each key points one by one let player know what's going to happen
//step 3.show whole path then fade out
//step 4.show start click animation
//step 5.start to click and drag the path
//step 6.while time's up to player finish the click and drag check the score
//
//Jan/07/2022
//now I just want to show hint so it's lazy coding now.
class cGamePlayAttackPath:public cCurveWithTime
{
	enum eStatus
	{
		eSHOW_COUNT_DOWN_ANIMATION = 0,
		eSHOW_KEY_POINTS_BY_ORDER,
		eSHOW_WHOLE_PATH_AND_FADE_OUT,
		eSHOW_SHOW_READY_TO_PLAY_ANIMATION,
		eSHOW_PLAYER_INTERACTIVE,
		eSHOW_FINISH,
		eSHOW_MAX
	};
	//
	eStatus				m_eStatus;
	float				m_fPlayerClickMatchPercentage;
	//
	struct sShowKeyPoints
	{
		sShowKeyPoints();
		~sShowKeyPoints();
		int					m_iCurrentKeyPoints;
		UT::sTimeCounter	m_TCForFadeOut;
		cNumeralImage*		m_pNumeralImage;
		void				Update();
		void				sRender();
	};
	sShowKeyPoints*			m_pShowKeyPoints;
public:
	cGamePlayAttackPath();
	~cGamePlayAttackPath();
	virtual void	Update(float e_fElpaseTime)override;
	virtual void	Render()override;
};