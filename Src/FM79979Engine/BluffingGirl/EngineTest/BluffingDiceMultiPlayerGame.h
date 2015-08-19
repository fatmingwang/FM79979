#ifndef	_BLUFFING_DICE_MULTI_PLAYER_GAME_PHASE_
#define	_BLUFFING_DICE_MULTI_PLAYER_GAME_PHASE_


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

class	cBluffingDiceMultiPlayerGame:public cSimplePhase,public ISAXCallback
{
	enum	eGameStatus
	{
		eGS_DICE_SHAKE = 0,
		e_GAME_PLAY,
		e_WAITING_PLAYER_DICE_DATA,
		e_WAITING_START,
		e_NETWORK_ERROR,
	};
	eGameStatus								m_eGameStatus;
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
	//
	cSpecialDiceRule*						m_pSpecialDiceRule;
	int										m_iCallDicePoint;
	int										m_iCallDiceCountIndex;
	//
	cMPDI*									m_pShowMPDI;
	cMPDI*									m_pResultMPDI;
	//
	cSubMPDI*								m_pPlayerCallSubMPDI;
	cSubMPDI*								m_pAICallSubMPDI;
	cSubMPDI*								m_pAICatchSubMPDI;
	cSubMPDI*								m_pPlayerCatchSubMPDI;
	cMPDI*									m_pPlayerWin;
	cMPDI*									m_pAIWin;
	cMPDI*									m_pBGMPDI;
	cMPDI*									m_pWaitingPlayerRespondMPDI;
	cMPDI*									m_pWhoAmIMPDI;
	cMPDI*									m_pPleaseCallMPDI;
	//
	cDiceTable*								m_pPlayerDiceTable;
	cImageButton*							m_pCallImageButton;
	cImageButton*							m_pCatchImageButton;
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
	void									CallDiceImageUpdate(int e_iCount,int e_iPoint,bool e_bPlaTC,int e_iTurnIndex);
	void									UpdateDiceCall(float e_fElpaseTime);
	void									UpdateDiceCatch(float e_fElpaseTime);
	void									ChangeDiceResultImage();
	void									ChangeDiceShowImage();
	void									TurnIndexChange(int e_iPlayerIndex);
	//
	void									DoCatch(bool e_bMyCall);
	int										m_iMyIndexOfPlayers;
	int										m_iWhichPlayerCatch;
	int										m_iWhichPlayerIsBluffing;
	void									ProcessNetworkMessage(float e_fElpaseTime);
	void									SendMessage(int e_iMessageID,int e_iWhichPlayerSentIndex = -1,int e_iDiceCount = -1,int e_iDicePoint = -1);
	std::vector<int>						m_iIAmReadyMessageVector;
	bool									m_bReceivedCatchMessage;
	//
	cMPDI*									m_pOrtherPlayerCallDiceMPDI;
	cSubMPDI*								m_pOrtherPlayerCallDicePoint;
	cSubMPDI*								m_pOrtherPlayerCallDiceCount;
	cSubMPDI*								m_pOrtherPlayerCallIndex;
	UT::sTimeCounter						m_NetworkErrorTC;
public:
	cBluffingDiceMultiPlayerGame();
	~cBluffingDiceMultiPlayerGame();
	virtual	void			Destroy();
	virtual	void			Update(float e_fElpaseTime);
	virtual	void			Init();							//it would be called if the stage is changed.
	virtual	void			Render();
	virtual	void			DebugRender();
	virtual	void			MouseDown(int e_iX,int e_iY);
	virtual	void			MouseUp(int e_iX,int e_iY);
	virtual	void			MouseMove(int e_iX,int e_iY);
	virtual	const			WCHAR*	GetNextPhaseName();
	cBluffingDiceAI*		GetBluffingDiceAI(){return m_pBluffingDiceAI;}
	virtual	void			FetchData(const WCHAR*e_strPhaseName,void*e_pData);
};

#endif