#ifndef	_BLUFFING_DICE_GAME_PHASE_
#define	_BLUFFING_DICE_GAME_PHASE_


#include "ChoiceGirlStagePhase.h"
//<>
//<DiceTableData FileName="">
//<AIData WinCallPercentage="0.6" SkipStep="1,2" VisibleDice="5,6" />
//

class	cDiceTable;
class	cBluffingDiceAI;
class	cAIDiceTableCallResult;
class	cSpecialDiceRule;
struct	sDiceRule;

class	cBluffingDiceGame:public cSimplePhase,public ISAXCallback
{
	class	cBattleRoundData
	{
		cMPDI*				m_pHintMPDI;
	public:
		cBattleRoundData();
		~cBattleRoundData();
		void				RefreshResult();
		void				Init();
		void				Destroy();
		void				Render();
		void				RoundAdd();
		void				ResultLog(int e_iWinIndex);
		int					m_iNumRound;
		int					m_iPlayerWinCount;
		int					m_iAIWinCount;
		int					m_iTotalRound;
		int					m_iWinCount;
		int					m_iCurrentWinCount;
		int					m_iCurrentLoseCount;
	};
	enum	eGameStatus
	{
		eGS_DICE_SHAKE = 0,
		eGS_CLAIRVOYANT_USE_CHOICE,
		e_GAME_PLAY,
		eGS_RESULT_VIDEO,//win or lose
	};
	eGameStatus								m_eGameStatus;
	cFMVVideo*								m_pVideo;
	cChoiceGirlStagePhase::cStageData*		m_pStageData;
	void									WriteWinMessage(int e_iWinIndex);
	enum	eCatchType
	{
		eCT_AI_CATCH = 0,
		eCT_PLAYER_CATCH,
		eCT_NO_CATCH
	};

	cResourceStamp							m_ResourceStamp;
	std::string								m_strGameSetupFileName;
	std::wstring							m_strNextPhaseName;
	std::wstring							m_strBGFileName;
	std::wstring							m_strVideoFileName;
	bool									m_bLastStage;//if true all stages are clear
	cBaseImage*								m_pBGImage;
	cBasicSound*							m_pDiceCallSound;
	//
	cBattleRoundData*						m_pBattleRoundData;
	cSpecialDiceRule*						m_pSpecialDiceRule;
	int										m_iCallDicePoint;
	int										m_iCallDiceCountIndex;
	//
	cPhasePopUpMessager*					m_pUseClairvoyant;
	cMPDI*									m_pShowUseClairvoyantMPDI;
	cMPDI*									m_pShowNotUseClairvoyantMPDI;
	cMPDI*									m_pShowMPDI;
	//
	cMPDI*									m_pResultUseClairvoyantMPDI;
	cMPDI*									m_pResultNotUseClairvoyantMPDI;
	cMPDI*									m_pResultMPDI;
	//
	cSubMPDI*								m_pPlayerCallSubMPDI;
	cSubMPDI*								m_pAICallSubMPDI;
	cSubMPDI*								m_pAICatchSubMPDI;
	cSubMPDI*								m_pPlayerCatchSubMPDI;
	cMPDI*									m_pPlayerWin;
	cMPDI*									m_pAIWin;
	cMPDI*									m_pBGMPDI;
	//
	cDiceTable*								m_pPlayerDiceTable;
	cImageButton*							m_pCallImageButton;
	cImageButton*							m_pCatchImageButton;
	cImageButton*							m_pShowRuleButton;
	cClickMouseBehaviorVector<cImageButton>*m_pFailedMenuButtonList;
	//m_AIDiceTable m_pBluffingDiceAI conbine together,here AI not create as many many as m_AIDiceTable's size,
	//its because all AI should be same data or it's easy to fail by one of them.
	std::vector<cAIDiceTableCallResult*>	m_AIDiceTableCallResultVector;
	cBluffingDiceAI*						m_pBluffingDiceAI;
	//
	sDiceRule*								m_pDiceRule;
	//
	virtual	void							HandleElementData(TiXmlElement*e_pTiXmlElement);
	void									ProcessDiceRule(TiXmlElement*e_pTiXmlElement);
	void									ProcessDiceTableData(TiXmlElement*e_pTiXmlElement);
	void									ProcessDiceAIData(TiXmlElement*e_pTiXmlElement);
	void									ProcessImageData(TiXmlElement*e_pTiXmlElement);
	void									ProcessSoundData(TiXmlElement*e_pTiXmlElement);
	void									ProcessPlayRoundData(TiXmlElement*e_pTiXmlElement);
	//
	int										m_iTurnToCallIndex;//0 for player,1-n for AI,if reach m_AIDiceTable.size back to 0
	//
	cPuzzleImage*							m_pGameResultImagePI;
	eCatchType								m_eCatchType;
	int										m_iWinIndex;//0 for player
	bool									IsCatchWin();
	//
	void									CallDiceImageUpdate(int e_iCount,int e_iPoint,bool e_bPlayer);
	void									UpdateUseChairvoyant(float e_fElpaseTime);
	void									UpdateDiceCall(float e_fElpaseTime);
	void									UpdateDiceCatch(float e_fElpaseTime);
	void									UpdateDiceShakeVideo(float e_fElpaseTime);
	void									UpdateResultVideo(float e_fElpaseTime);
	void									ChangeDiceResultImage();
	void									ChangeDiceShowImage();
	//
	std::wstring							m_strCurrentSelectedGirlNameAndStageName;
	std::wstring							m_strUnlockGirlName;
	//
	void									GameRestartInit();
	//
public:
	cBluffingDiceGame();
	~cBluffingDiceGame();
	virtual	void			Destroy();
	virtual	void			Update(float e_fElpaseTime);
	virtual	void			Init();							//it would be called if the stage is changed.
	virtual	void			Render();
	virtual	void			DebugRender();
	virtual	void			MouseDown(int e_iX,int e_iY);
	virtual	void			MouseUp(int e_iX,int e_iY);
	virtual	void			MouseMove(int e_iX,int e_iY);
	virtual	void			KeyUp(char e_cKey);
	virtual	const			WCHAR*	GetNextPhaseName();
	cBluffingDiceAI*		GetBluffingDiceAI(){return m_pBluffingDiceAI;}
	virtual	void*			GetData();
	virtual	void			FetchData(const WCHAR*e_strPhaseName,void*e_pData);
};

#endif