/* ============================================================================================= *\
FILE NAME   : 
DESCRIPTION : 
CREATED BY  : 
HISTORY     : 
\* ============================================================================================= */
#include "stdafx.h"
#include "DeviceRefundCoinBase.h"//
#include "../GameDefine//FishGameEventMessageID.h"
#include "../Player/PlayerMessageBehavior.h"
#include "../ControlPanel/ControlSettingParameter.h"
#include "../GameDefine/GameParameterDefine.h"

//#include "../PlayerBehavior/PlayerManager.h"
//#include "../../IOSimulationMachine/8051/GH8051.h"
//
//#include "../ProbabilityFish/ProbabilityFish.h"
//
int		g_iCurrentSelectedPlayerIndex = -10;
UT::sTimeAndFPS	cDeviceRefoundCoinBase::m_sRefundCoinGap;
cDeviceRefoundCoinBase::cDeviceRefoundCoinBase(
										int	e_iScoreToCoinOrTicketExchangeRate,
										sScoreBehaviorStruct*e_pScoreBehaviorStruct,
										cIOPackData*e_pInsertCoin,//玩家投幣的數量
										cIOPackData*e_pRefundWriteDevice,//ticket or coin refund write device
										cIOPackData*e_pRefundReadDevice,//ticket or coin refund read device
										cIOBitData*	e_pResetRefundCountDevice,
										cIOBitData*	e_pPrintIOError,
										unsigned char e_ucRefundKey,//
										unsigned char e_ucExchangeInKey,
										unsigned char e_ucExchangeOutKey,
										HowManyScoreToRefundFunction	e_HowManyScoreToRefundFunction
									   )
{
	m_i64PrintRefundValue = 0;
	m_bPrintModeWait2SecondsFlag = false;
	m_PrintModeWait2SecondsTC.SetTargetTime(PRINT_MODE_WAIT_2_SECONDS);
	m_iScoreToCoinOrTicketExchangeRate = e_iScoreToCoinOrTicketExchangeRate;
	m_iRefundCoinStartFlag = 0;
	m_i64RestCoinToRefund = 0;
	m_bRefundCoin = false;
	m_pCoinToastTestForDeviceNotExist = nullptr;
	m_pScoreBehaviorStruct = e_pScoreBehaviorStruct;
	m_pInsertCoin = e_pInsertCoin;
	m_pRefundWriteDevice = e_pRefundWriteDevice;
	m_pRefundReadDevice = e_pRefundReadDevice;
	m_pResetRefundCountDevice = e_pResetRefundCountDevice;
	m_HowManyScoreToRefundFunction = e_HowManyScoreToRefundFunction;
	m_pPrintIOError = e_pPrintIOError;
	assert(m_pInsertCoin);
	//changSend IOBoard dont have this
	//assert(m_pRefundWriteDevice);
	//assert(m_pRefundReadDevice);
	//assert(m_pResetRefundCountDevice);
	if(m_pRefundWriteDevice)
		m_pRefundWriteDevice->m_IOSignalWriteFunction = std::bind(&cDeviceRefoundCoinBase::WriteRefundSignal, this);
	if(m_pRefundReadDevice)
		m_pRefundReadDevice->m_IOSignalReadedFunction = std::bind(&cDeviceRefoundCoinBase::ReadRefundSignal, this);
	m_RefundKey.SetKeyData(e_ucRefundKey);
	m_ExchangeInKey.SetKeyData(e_ucExchangeInKey);
	m_ExchangeOutKey.SetKeyData(e_ucExchangeOutKey);

	if (!cGameApp::m_sbDeviceExist)
	{
		m_ExchangeInKey.SetKeyData('B');
		m_ExchangeOutKey.SetKeyData('N');
		m_pCoinToastTestForDeviceNotExist = new cClickMouseBehavior();
		m_pCoinToastTestForDeviceNotExist->SetKeyData('V');
		m_pCoinToastTestForDeviceNotExist->SetPressedRepeatMouseUp(true);
	}

	m_bWorking = false;
	m_pBinaryFile = 0;
	m_RefundToCheckCoinTC.SetTargetTime(REFUND_TIME_OUT_TIME);
	m_sRefundCoinGap.Update();
	//m_Wait8051SingnalToActive.SetTargetTime(REFRESH_8051_IO_TIME_OFFSET);
}

/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : 
DESCRIPTION   : 
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */
cDeviceRefoundCoinBase::~cDeviceRefoundCoinBase() 
{
	SAFE_DELETE(m_pCoinToastTestForDeviceNotExist);
}

void	cDeviceRefoundCoinBase::DoScoreChangeFunctionVector(int64 e_i64Count, eMoneyChangeType e_eMoneyChangeType)
{
	int64 l_i64Score = e_i64Count;
	if (eMoneyChangeType::eMCT_COIN_REFUND_MINUS_SCORE != e_eMoneyChangeType && eMoneyChangeType::eMCT_TICKET_REFUND_MINUS_SCORE != e_eMoneyChangeType)
		l_i64Score = this->m_iScoreToCoinOrTicketExchangeRate*e_i64Count;

	size_t l_uiSize = m_ScoreChangeFunctionVector.size();
	for (size_t i = 0; i < l_uiSize; ++i)
	{
		auto l_Function = m_ScoreChangeFunctionVector[i];
		l_Function(l_i64Score, e_eMoneyChangeType);
	}
}

extern int	g_iCurrentSelectedPlayerIndex;
void	cDeviceRefoundCoinBase::Update_ExchangeBehaviod( float e_fElpaseTime )
{
	//while coin is during refunding,refuse exchange button behavior
	if (!cGameApp::m_sbDeviceExist || *m_pScoreBehaviorStruct->pbIsRefundingCoin)
	{
		if (m_pScoreBehaviorStruct->iPlayerID != g_iCurrentSelectedPlayerIndex && g_iCurrentSelectedPlayerIndex != -1)
		{
			return;
		}
	}
	float	l_fLongPressTime = 1.f;
	float	l_fMouseDownElpaseTime = m_ExchangeInKey.GetMouseMoveData().GetDownElpaseTime();
	if (sControlSettingParameter::m_siAddScoreIOBehaviod != 2)
	{
		m_ExchangeInKey.SingnalProcess();
		m_ExchangeInKey.Update(e_fElpaseTime);

		if (m_ExchangeInKey.IsSatisfiedCondition() || l_fMouseDownElpaseTime > l_fLongPressTime)
		{
			int64 l_i64InCount = 0;
			if (l_fMouseDownElpaseTime > l_fLongPressTime)
			{
#ifdef DEBUG
				l_i64InCount = 1000;
#else
				l_i64InCount = 100;
#endif
				DoScoreChangeFunctionVector(l_i64InCount, eMoneyChangeType::eMCT_LONG_PRESS_BUTTON_ADD_SCORE);
			}
			else
			{
				//l_i64InCount = cFishApp::m_pGameSetup->GetKeyinPower() * 10;//Set KeyInPower from CommonGameSetup.xml, for speed add score.
				l_i64InCount = 10;
				DoScoreChangeFunctionVector(l_i64InCount, eMoneyChangeType::eMCT_PRESS_BUTTON_ADD_SCORE);
			}
			m_ExchangeInKey.Init();
		}
	}

	m_ExchangeOutKey.SingnalProcess();
	m_ExchangeOutKey.Update(e_fElpaseTime);
	
	l_fMouseDownElpaseTime = m_ExchangeOutKey.GetMouseMoveData().GetDownElpaseTime();
	if( m_ExchangeOutKey.IsSatisfiedCondition() || l_fMouseDownElpaseTime > l_fLongPressTime )
	{
		int64	l_i64PlayerScore = *m_pScoreBehaviorStruct->pi64Score;
		int64	l_i64OutCount = 0;
		int		l_iCoinRate = sControlSettingParameter::m_siCoinToScoreRate;
		int		l_iAllWashScoreUnit = l_iCoinRate * 10;
		bool	l_bCheck = (bool)m_pScoreBehaviorStruct->m_IsDeleteMessageOnFunction();
		if ( l_bCheck )
		{
			DoScoreChangeFunctionVector(l_i64OutCount, eMoneyChangeType::eMCT_PRESS_BUTTON_MINUS_SCORE);
		}
		else 
		if( l_fMouseDownElpaseTime > l_fLongPressTime )
		{
			if (l_i64PlayerScore)
			{
				l_i64PlayerScore %= l_iAllWashScoreUnit;
				l_i64OutCount = (*this->m_pScoreBehaviorStruct->pi64Score - l_i64PlayerScore) / l_iCoinRate;
			}
			DoScoreChangeFunctionVector(l_i64OutCount, eMoneyChangeType::eMCT_ALL_WASH_MINUS_SCORE);
		}
		else
		{
			if (l_i64PlayerScore >= l_iAllWashScoreUnit)
				l_i64OutCount = 10;
			DoScoreChangeFunctionVector(l_i64OutCount, eMoneyChangeType::eMCT_PRESS_BUTTON_MINUS_SCORE);
		}
		cGameApp::m_spMessageSenderManager->EventMessageShot(eFGEMI_REGISTER_OUT, (char*)&l_i64OutCount, sizeof(int64));
		m_ExchangeOutKey.Init();
		*m_pScoreBehaviorStruct->pbCoinNotEnoughToRefund = false;
	}
}


void	cDeviceRefoundCoinBase::UpdateTostCoinBehavior(float e_fElpaseTime)
{
	//Tost coin
	if (sControlSettingParameter::m_siAddScoreIOBehaviod != 3)
	{
		int l_iTostCoinCount = this->m_pInsertCoin->GetDataByByte();
		void*l_pData = this->m_pInsertCoin->GetData();
		*(unsigned char*)l_pData = 0;
		if (l_iTostCoinCount > 0)
			DoScoreChangeFunctionVector(l_iTostCoinCount, eMoneyChangeType::eMCT_COIN_TOST_ADD_SCORE);
	}
	else
	if (m_pCoinToastTestForDeviceNotExist)
	{//detect tost coin key
	 //while coin is during refunding,refuse exchange button behavior
		if (!cGameApp::m_sbDeviceExist || *m_pScoreBehaviorStruct->pbIsRefundingCoin)
		{
			if (m_pScoreBehaviorStruct->iPlayerID != g_iCurrentSelectedPlayerIndex && g_iCurrentSelectedPlayerIndex != -1)
			{
				return;
			}
		}
		m_pCoinToastTestForDeviceNotExist->SingnalProcess();
		m_pCoinToastTestForDeviceNotExist->Update(e_fElpaseTime);
		if (m_pCoinToastTestForDeviceNotExist->IsSatisfiedCondition())
		{
			DoScoreChangeFunctionVector(1, eMoneyChangeType::eMCT_COIN_TOST_ADD_SCORE);
			m_pCoinToastTestForDeviceNotExist->Init();
		}
	}
}

bool	cDeviceRefoundCoinBase::WriteRefundSignal()
{
	if (m_iRefundCoinStartFlag == 0)
	{
		this->m_iRefundCoinStartFlag = 1;
	}
	return true;
}
bool	cDeviceRefoundCoinBase::ReadRefundSignal()
{
	if (this->m_iRefundCoinStartFlag >= 1)
	{
		++this->m_iRefundCoinStartFlag;
	}
	return true;
}

void	cDeviceRefoundCoinBase::SendCoinRefundSingnal(bool e_bFirstTime)
{
	*m_pScoreBehaviorStruct->pbCoinNotEnoughToRefund = false;
	if (cGameApp::m_sbDeviceExist)
	{
		int64 l_i64RefundCount = 0;
		if (m_HowManyScoreToRefundFunction)
			l_i64RefundCount = m_HowManyScoreToRefundFunction();
		if (l_i64RefundCount >= this->m_iScoreToCoinOrTicketExchangeRate)//money is enough to refund.
		{
			m_RefundToCheckCoinTC.Start();
			m_bRefundCoin = true;
			m_i64RestCoinToRefund = l_i64RefundCount / this->m_iScoreToCoinOrTicketExchangeRate;

			int l_iOutMode = sControlSettingParameter::m_siScoreToMoneyIOMode;
			if (l_iOutMode == REFUND_MODE_PRINT)
			{
				if (e_bFirstTime)
				{
					if (m_i64RestCoinToRefund > SAFE_PRINT_COUNT)
					{
						m_i64RestCoinToRefund = SAFE_PRINT_COUNT;
					}
					m_i64PrintRefundValue = m_i64RestCoinToRefund;
					if(m_pRefundWriteDevice)
						this->m_pRefundWriteDevice->SetData((short)m_i64RestCoinToRefund);
				}
				else
				{
					this->m_bPrintModeWait2SecondsFlag = true;
					m_PrintModeWait2SecondsTC.Start();
				}
			}
			else
			{
				if (m_i64RestCoinToRefund > SAFE_REFUND_COIN_COUNT)
				{
					m_i64RestCoinToRefund = SAFE_REFUND_COIN_COUNT;
				}
				if(m_pRefundWriteDevice)
					this->m_pRefundWriteDevice->SetData((unsigned char)m_i64RestCoinToRefund);
			}
			m_iRefundCoinStartFlag = 0;
		}
		else//not enough money.
		{
			m_bRefundCoin = false;
		}
	}
	else
	{
		m_bRefundCoin = false;
		//
		int64 l_i64RefundCount = 0;
		if (m_HowManyScoreToRefundFunction)
			l_i64RefundCount = m_HowManyScoreToRefundFunction();
		DoScoreChangeFunctionVector(l_i64RefundCount, eMoneyChangeType::eMCT_COIN_REFUND_MINUS_SCORE);
		//
		m_pScoreBehaviorStruct->pi64Score = 0;
	}
}

void	cDeviceRefoundCoinBase::UpdateRefundBehavior(float e_fElpaseTime)
{
	*m_pScoreBehaviorStruct->pbIsRefundingCoin = m_bRefundCoin;
	if (m_bRefundCoin)
	{
		if (m_iRefundCoinStartFlag >= START_REFUND_FLAG_VALUE)
		{
			int64 l_i64CurrentRefundCoin = 0;
			int l_iOutMode = sControlSettingParameter::m_siScoreToMoneyIOMode;
			if (m_pRefundReadDevice)
			{
				if (m_pRefundReadDevice->GetDataSize() == 1)
				{
					l_i64CurrentRefundCoin = this->m_pRefundReadDevice->GetDataByByte();
					assert(l_iOutMode == REFUND_MODE_COIN || l_iOutMode == REFUND_MODE_TICKER);
				}
				else
				if (m_pRefundReadDevice->GetDataSize() == 2)
				{
					short l_shValue = m_pRefundReadDevice->GetDataByShort();
					l_i64CurrentRefundCoin = l_shValue;
					assert(l_iOutMode == REFUND_MODE_PRINT);
				}
				else
				{
					//not expect this.
					m_RefundToCheckCoinTC.bTragetTimrReached = true;
				}
			}
			else
			{
				//this IO dont support this mode
				m_RefundToCheckCoinTC.bTragetTimrReached = true;
				//assert(0 && "m_pRefundReadDevice->GetDataSize() is not 1 or 2");
			}
			if (m_i64RestCoinToRefund != l_i64CurrentRefundCoin)
			{
				int64 l_i64NumCoinRefune = m_i64RestCoinToRefund - l_i64CurrentRefundCoin;
				int64 l_i64RefundScore = l_i64NumCoinRefune*this->m_iScoreToCoinOrTicketExchangeRate;
				if (l_iOutMode == REFUND_MODE_PRINT)
				{
					m_RefundToCheckCoinTC.Start();
					if (l_i64CurrentRefundCoin == 0)
					{
						//do nothing here need to do this after m_PrintModeWait2SecondsTC's time up
						//DoScoreChangeFunctionVector(m_i64RestCoinToRefund, eMoneyChangeType::eMCT_PRINT_OUT_SCORE);//////////sean
						//m_i64RestCoinToRefund = 0;
						m_i64RestCoinToRefund = 0;
					}
				}
				else
				if (l_iOutMode == REFUND_MODE_COIN || l_iOutMode == REFUND_MODE_TICKER)
				{
					m_i64RestCoinToRefund -= l_i64NumCoinRefune;
					if (l_iOutMode == REFUND_MODE_COIN)
						DoScoreChangeFunctionVector(l_i64RefundScore, eMoneyChangeType::eMCT_COIN_REFUND_MINUS_SCORE);
					else
					if (l_iOutMode == REFUND_MODE_TICKER)
						DoScoreChangeFunctionVector(l_i64RefundScore, eMoneyChangeType::eMCT_TICKET_REFUND_MINUS_SCORE);
				}
				else
					assert(0 && "cFishApp::m_spControlPanel->GetOutMode() is  not 0 or 1 or 2");
				//current wanted coin refund is done,want more coin to refund?
				if (m_i64RestCoinToRefund <= 0)
				{
					SendCoinRefundSingnal(false);
					m_RefundToCheckCoinTC.Start();
				}
			}
		}
		 //motor spin too long,no coin
		if (m_RefundToCheckCoinTC.bTragetTimrReached)
		{
			m_bRefundCoin = false;
			*m_pScoreBehaviorStruct->pbCoinNotEnoughToRefund = true;
			if(m_pResetRefundCountDevice)
				m_pResetRefundCountDevice->SetDataByBoolean(true);
		}
	}
	else
	{//check refund key pressed
		m_RefundKey.SingnalProcess();
		m_RefundKey.Update(e_fElpaseTime);
		if (m_RefundKey.IsSatisfiedCondition())
		{
			m_sRefundCoinGap.Update();
			m_RefundKey.Init();
			if (m_sRefundCoinGap.fElpaseTime >= REFUND_PRESS_TIME)
			{
				SendCoinRefundSingnal(true);
			}
				
		}
	}
}

void	cDeviceRefoundCoinBase::Update_CoinBehavior( float e_fElpaseTime )
{
	UpdateTostCoinBehavior(e_fElpaseTime);
	if (m_bPrintModeWait2SecondsFlag)
	{
		m_PrintModeWait2SecondsTC.Update(e_fElpaseTime);
		if (m_PrintModeWait2SecondsTC.bTragetTimrReached)
		{
			//ensure print works fine
			if (!this->m_pPrintIOError->IsSingnalActive())
			{
				DoScoreChangeFunctionVector(m_i64PrintRefundValue, eMoneyChangeType::eMCT_PRINT_OUT_SCORE);
				m_i64PrintRefundValue = 0;
				m_i64RestCoinToRefund = 0;
				SendCoinRefundSingnal(true);
			}
			m_bPrintModeWait2SecondsFlag = false;
		}
	}
	else
	{
		UpdateRefundBehavior(e_fElpaseTime);
	}

}
void	cDeviceRefoundCoinBase::AddScoreChangeFunction(ScoreChangeFunction e_ScoreChangeFunctionVector)
{
	m_ScoreChangeFunctionVector.push_back(e_ScoreChangeFunctionVector);
}

void cDeviceRefoundCoinBase::Update( float e_fElpaseTime )
{
	//if( g_pIOSMDriverInterface->IsIOError() || cFishApp::m_bShowReportTipImage  )
	{
		//return;
	}
	//
	Update_CoinBehavior( e_fElpaseTime );
	//
	Update_ExchangeBehaviod( e_fElpaseTime );
	//
	if( m_bRefundCoin )
	{
		m_RefundToCheckCoinTC.Update(e_fElpaseTime);
	}
}

void cDeviceRefoundCoinBase::Render(int e_iPosX, int e_iPosY)
{
	std::wstring	l_str = UT::ComposeMsgByFormat(L"RefundFlag:%d", m_iRefundCoinStartFlag);
	cGameApp::m_spGlyphFontRender->RenderFont(e_iPosX, e_iPosY, l_str.c_str());
	//swprintf(m_strReceive, MAX_PATH, L"Refund State = %d",m_iDebugState);
	//cGameApp::m_spGlyphFontRender->RenderFont(800 , 800, m_strReceive);
}

void cDeviceRefoundCoinBase::RefundTest(int e_iCount)
{
	//this->m_piScoreToCoinOrTicketExchangeRate
}

bool	cDeviceRefoundCoinBase::IsRefund()
{
	if (this->m_pScoreBehaviorStruct)
	{
		return *this->m_pScoreBehaviorStruct->pbIsRefundingCoin;
	}
	return false;
}