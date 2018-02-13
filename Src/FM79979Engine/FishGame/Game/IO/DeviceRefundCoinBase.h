#ifndef _cDeviceRefoundCoinBase_H_
#define _cDeviceRefoundCoinBase_H_
#include "Comport/IOData.h"
//

#define SAFE_REFUND_COIN_COUNT		5
#define START_REFUND_FLAG_VALUE		5
#define REFUND_TIME_OUT_TIME		6.f
#define REFUND_PRESS_TIME			0.3f
#define PRINT_MODE_WAIT_2_SECONDS	5.f

#define SAFE_PRINT_COUNT			50000

enum eMoneyChangeType;

//probability and player should do a calculation with exchange rate.
typedef	std::function<bool(int64 e_iCount, eMoneyChangeType e_eMoneyChangeType)>	ScoreChangeFunction;

typedef	std::function<int64()>													HowManyScoreToRefundFunction;


struct sScoreBehaviorStruct
{
	int						iPlayerID;
	int64*					pi64Score;
	bool*					pbCoinNotEnoughToRefund;
	bool*					pbIsRefundingCoin;
	std::function<bool()>	m_IsDeleteMessageOnFunction;
	sScoreBehaviorStruct(int64*e_pi64Score, bool*e_pbCoinNotEnoughToRefund, bool*e_pbIsRefundingCoin,int e_iPlayerID
		, std::function<bool()> e_bIsDeleteMessage)
	{
		iPlayerID = e_iPlayerID;
		pi64Score = e_pi64Score;
		pbCoinNotEnoughToRefund = e_pbCoinNotEnoughToRefund;
		pbIsRefundingCoin = e_pbIsRefundingCoin;
		m_IsDeleteMessageOnFunction = e_bIsDeleteMessage;
	}
	~sScoreBehaviorStruct() {}
};


class	cDeviceRefoundCoinBase : public NamedTypedObject
{
	//avoid multi-player refund at same time
	static UT::sTimeAndFPS	m_sRefundCoinGap;
private:
	int64					m_i64RestCoinToRefund;
	int64					m_i64PrintRefundValue;//because print need time and some player just stuid(toast while print)
	//over than 5 second show no coin messae
	UT::sTimeCounter		m_RefundToCheckCoinTC;
	//UT::sTimeCounter m_Wait8051SingnalToActive;
	bool					m_bRefundCoin;
	//0->1->2->3...if value bigger than 5 refund is start,it should be 2,
	//but somehow IO board has some problem with sync...so I will start to refund while value is bigger than 5
	int						m_iRefundCoinStartFlag;
	//ensure IO written and IO readed,then start count coin refund
	bool					WriteRefundSignal();
	bool					ReadRefundSignal();
	cBinaryFile*			m_pBinaryFile;
	bool					m_bWorking;//bool of return	

	cIOPackData*			m_pInsertCoin;//玩家投幣的數量
	cIOPackData*			m_pRefundWriteDevice;//ticket or coin or print
	cIOPackData*			m_pRefundReadDevice;//
	cIOBitData*				m_pResetRefundCountDevice;//reset refund device value as expect so device keep walking
	cIOBitData*				m_pPrintIOError;//rest print value is zero then wait for 1.5 seconds to check print error if no error do minus player score

	WCHAR					m_strReceive[MAX_PATH];
	int						m_iScoreToCoinOrTicketExchangeRate;//how many score to one coin tor ticket
	cClickMouseBehavior		m_RefundKey;//
	cClickMouseBehavior		m_ExchangeInKey;//
	cClickMouseBehavior		m_ExchangeOutKey;//
	cClickMouseBehavior*	m_pCoinToastTestForDeviceNotExist;//for device not exists
	void					Update_ExchangeBehaviod( float e_fElpaseTime );
	void					UpdateTostCoinBehavior(float e_fElpaseTime);
	bool					m_bPrintModeWait2SecondsFlag;
	UT::sTimeCounter		m_PrintModeWait2SecondsTC;
	void					UpdateRefundBehavior(float e_fElpaseTime);
	void					Update_CoinBehavior( float e_fElpaseTime );
	void					SendCoinRefundSingnal(bool e_bFirstTime);
	//
	sScoreBehaviorStruct*				m_pScoreBehaviorStruct;
	//
	std::vector<ScoreChangeFunction>	m_ScoreChangeFunctionVector;
	void								DoScoreChangeFunctionVector(int64 e_i64Count,eMoneyChangeType e_eMoneyChangeType);
	//
	HowManyScoreToRefundFunction			m_HowManyScoreToRefundFunction;
public:
	cDeviceRefoundCoinBase(
						int	e_iScoreToCoinOrTicketExchangeRate,
						sScoreBehaviorStruct*e_pScoreBehaviorStruct,
						cIOPackData*e_pInsertCoin,//玩家投幣的數量
						cIOPackData*e_pRefundWriteDevice,//ticket or coin refund write device
						cIOPackData*e_pRefundReadDevice,//ticket or coin refund read device
						cIOBitData*	e_pResetRefundCountDevice,
						cIOBitData*	e_pPrintIOError,
						unsigned char e_ucRefundKey,//退幣紐
						unsigned char e_ucOpenKey,//開分紐
						unsigned char e_ucWashKey,//洗分紐
						HowManyScoreToRefundFunction	e_HowManyScoreToRefundFunction
					  );
	~cDeviceRefoundCoinBase();
	void					AddScoreChangeFunction(ScoreChangeFunction e_ScoreChangeFunctionVector);
	void					Update(float e_fElpaseTime);
	void					Render(int e_iPosX,int e_iPosY);
	void					RefundTest(int e_iCount);
	bool					IsRefund();
};

#endif