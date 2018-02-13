#pragma once

enum eMoneyChangeType
{
	eMCT_COIN_TOST_ADD_SCORE = 0,//inserted coin
	eMCT_COIN_REFUND_MINUS_SCORE,
	eMCT_TICKET_REFUND_MINUS_SCORE,
	eMCT_PRESS_BUTTON_MINUS_SCORE,
	eMCT_PRESS_BUTTON_ADD_SCORE,
	eMCT_LONG_PRESS_BUTTON_ADD_SCORE,
	eMCT_FIRE_BULLET_MINUS_SCORE,
	eMCT_FIRE_RETURN_BULLET_ADD_SCORE,//last game crush or something happen,put the bullets back to player
	eMCT_FISH_DIED_ADD_SCORE,//mini game...something else...
	eMCT_BIG_WIN_ADD_SCORE,
	eMCT_PRESS_MINI_GAME_ADD_SCORE,
	eMCT_DRAGON_BIG_WIN_ADD_SCORE,
	eMCT_ALL_WASH_MINUS_SCORE,//7,you have to calculate the score is enough for exchange rate
	eMCT_PRINT_OUT_SCORE,
	eMCT_CHECK_ACCOUNT,
	eMCT_MAX
};

enum ePlayerMessageType
{
	ePMT_IO_ERROR = 0,//IOError                             //I/O板連接錯誤
	ePMT_MONEY_LOCKED,//MoneyLocked_D						//鎖機
	ePMT_MONEY_NOT_ENOUGH,//MoneyNotEnough_D				//票幣不足
	//ePMT_REFUND_PROBLEM,//Hopper_D							//退幣馬達卡幣  Is this available???
	//ePMT_INSERT_COIN_PROBLEM,//CoinInsert_D					//投幣口卡幣	Is this available???
	ePMT_PRESS_BUTTON_ADD_SCORE_LIMIT,//CreditLimitError_D	//開分上限
	ePMT_PLEASE_INSERT_COIN,//PleaseInsertCoin_D			//請投幣
	ePMT_MAX,
};

class cPlayerMessageBehavior
{
protected:
	//
	struct sMessageStatus
	{
		cMPDI*						m_pMessageMPDI;
		bool						m_bMesageOn;
		sMessageStatus() { m_pMessageMPDI = nullptr; m_bMesageOn = false; }
		~sMessageStatus() { SAFE_DELETE(m_pMessageMPDI); }
	};
	sMessageStatus				m_MessageStatus[ePMT_MAX];
	bool						m_bCoinNotEnoughToRefund;//not enough coin or ticket for refund
	UT::sTimeCounter			m_PressButtonAddScoreLimitMPDIShowTime;//10 seconds? why
	float						m_ShowIOErrorWaitTime;//0.5 second sometimes IO just get wrong but get back immediatly,so wait for a while to show error message
	bool						InitMessageResource(cMatrix44 e_matShowMatrix, int64 e_i64ScoreLimit, int64 e_i64ButtonAddScoreLimit);
	//
protected:
	int64						m_i64Score;
	bool						m_bRefundingCoin;
	virtual void				UpdateMessageStatus(float e_fElpaseTime);
	virtual void				RenderMessageStatus();
	virtual bool				AddScoreByMoneyChangeType(eMoneyChangeType e_eMoneyChangeType, bool e_bMessageOn = false);
public:
	cPlayerMessageBehavior();
	virtual ~cPlayerMessageBehavior();
	bool						IsAllowToFire();
	bool*						GetCoinNotEnoughToRefundPointer() {return &m_bCoinNotEnoughToRefund;}
	bool*						GetRefundingCoinPointer() { return &m_bRefundingCoin; }
	bool						IsDeleteMessageOn();
	int64						GetScore(){ return m_i64Score; }
	int64*						GetScorePointer(){ return &m_i64Score; }
};