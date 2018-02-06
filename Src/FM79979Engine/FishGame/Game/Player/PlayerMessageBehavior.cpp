#include "stdafx.h"
#include "PlayerMessageBehavior.h"
#include "../GameDefine/GameDefine.h"


int64	cPlayerMessageBehavior::m_si64ScoreLimit = 10000000;
int64	cPlayerMessageBehavior::m_si64ButtonAddScoreLimit = 10000000;


cPlayerMessageBehavior::cPlayerMessageBehavior()
{
	m_bCoinNotEnoughToRefund = false;
	m_bRefundingCoin = false;
	m_ShowIOErrorWaitTime = SHOW_IO_ERROR_WAIT_TIME;
}
cPlayerMessageBehavior::~cPlayerMessageBehavior()
{


}

bool	cPlayerMessageBehavior::InitMessageResource(cMatrix44 e_matShowMatrix, int64 e_i64ScoreLimit, int64 e_i64ButtonAddScoreLimit)
{
	m_PressButtonAddScoreLimitMPDIShowTime.SetTargetTime(PRESS_BUTTON_ADD_SCORE_LIMIT_SHOW_MPDI_TIME);
	m_bCoinNotEnoughToRefund = false;
	m_bRefundingCoin = false;
	m_si64ScoreLimit = e_i64ScoreLimit;
	m_si64ButtonAddScoreLimit = e_i64ButtonAddScoreLimit;
	const wchar_t*l_strMEssageMPDIName[ePMT_MAX] =
	{
		L"IOError",
		L"MoneyLocked_D",
		L"MoneyNotEnough_D",
		//L"Hopper_D",
		//L"CoinInsert_D",
		L"CreditLimitError_D",
		L"PleaseInsertCoin_D"
	};
	cMPDIList*	l_pMessageMPDIList = NULL;
	auto l_strFileName = GetFishGameFileName(eFGFN_MESSAGE_MPDILIST);
	if(l_strFileName.length())
		l_pMessageMPDIList = cGameApp::GetMPDIListByFileName(UT::CharToWchar(l_strFileName).c_str());
	if (!l_pMessageMPDIList)
		return false;
	for (int i = 0; i<ePMT_MAX; ++i)
	{
		cMPDI*l_pMPDI = l_pMessageMPDIList->GetObject(l_strMEssageMPDIName[i]);
		if (l_pMPDI)
		{
			m_MessageStatus[i].m_pMessageMPDI = new cMPDI(l_pMPDI);
			m_MessageStatus[i].m_pMessageMPDI->Init();
			m_MessageStatus[i].m_pMessageMPDI->SetWorldTransform(e_matShowMatrix);
		}
		assert(l_pMPDI&&"InitMessageResource error!");
	}

	//check money reach limit.
	AddScoreByMoneyChangeType(eMCT_CHECK_ACCOUNT);
	return true;
}

void	cPlayerMessageBehavior::UpdateMessageStatus(float e_fElpaseTime)
{
	e_fElpaseTime = 0.016f;
	m_MessageStatus[ePMT_MONEY_NOT_ENOUGH].m_bMesageOn = this->m_bCoinNotEnoughToRefund;
	if (m_MessageStatus[ePMT_PRESS_BUTTON_ADD_SCORE_LIMIT].m_bMesageOn)
	{
		m_PressButtonAddScoreLimitMPDIShowTime.Update(e_fElpaseTime);
		if (m_PressButtonAddScoreLimitMPDIShowTime.bTragetTimrReached)
		{
			m_MessageStatus[ePMT_PRESS_BUTTON_ADD_SCORE_LIMIT].m_bMesageOn = false;
		}
	}

	//if (cFishApp::m_spIOControlInterface->IsIOError())
	//{
	//	if (m_ShowIOErrorWaitTime <= 0.f)
	//	{
	//		m_MessageStatus[ePMT_IO_ERROR].m_bMesageOn = true;
	//	}
	//	else
	//	{
	//		m_ShowIOErrorWaitTime -= e_fElpaseTime;
	//	}
	//}
	//else
	//{
	//	m_MessageStatus[ePMT_IO_ERROR].m_bMesageOn = false;
	//	m_ShowIOErrorWaitTime = SHOW_IO_ERROR_WAIT_TIME;
	//}
	for (int i = 0; i<ePMT_MAX; ++i)
	{
		if (m_MessageStatus[i].m_bMesageOn)
		{
			if(m_MessageStatus[i].m_pMessageMPDI)
			{
				m_MessageStatus[i].m_pMessageMPDI->Update(e_fElpaseTime);
			}
		}
	}
}

void	cPlayerMessageBehavior::RenderMessageStatus()
{
	for (int i = 0; i<ePMT_MAX; ++i)
	{
		if (m_MessageStatus[i].m_bMesageOn)
		{
			if (m_MessageStatus[i].m_pMessageMPDI)
			{
				m_MessageStatus[i].m_pMessageMPDI->Render();
				return;
			}
		}
	}
}


bool	cPlayerMessageBehavior::AddScoreByMoneyChangeType(eMoneyChangeType e_eMoneyChangeType, bool e_bMessageOn)
{
	int64 l_i64PlayerMoney = GetScore();
	switch (e_eMoneyChangeType)
	{
	case eMoneyChangeType::eMCT_ALL_WASH_MINUS_SCORE:
		m_MessageStatus[ePMT_MONEY_LOCKED].m_bMesageOn = false;
		m_MessageStatus[ePMT_PRESS_BUTTON_ADD_SCORE_LIMIT].m_bMesageOn = false;
		break;
	case eMoneyChangeType::eMCT_COIN_REFUND_MINUS_SCORE:
	case eMoneyChangeType::eMCT_TICKET_REFUND_MINUS_SCORE:
	case eMoneyChangeType::eMCT_PRESS_BUTTON_MINUS_SCORE:
	case eMoneyChangeType::eMCT_FIRE_BULLET_MINUS_SCORE:
		//if (l_i64PlayerMoney  < cFishApp::m_spControlPanel->GetScoreLimit())
		//{
		//	m_MessageStatus[ePMT_MONEY_LOCKED].m_bMesageOn = false;
		//}
		//if ( eMoneyChangeType::eMCT_PRESS_BUTTON_MINUS_SCORE == e_eMoneyChangeType)
		//{
		//	m_MessageStatus[ePMT_PRESS_BUTTON_ADD_SCORE_LIMIT].m_bMesageOn = false;
		//}
		break;
	case eMoneyChangeType::eMCT_COIN_TOST_ADD_SCORE:
	case eMoneyChangeType::eMCT_FISH_DIED_ADD_SCORE:
	case eMoneyChangeType::eMCT_PRESS_BUTTON_ADD_SCORE:
	case eMoneyChangeType::eMCT_BIG_WIN_ADD_SCORE:
	case eMoneyChangeType::eMCT_LONG_PRESS_BUTTON_ADD_SCORE:
	case eMoneyChangeType::eMCT_PRINT_OUT_SCORE:
	case eMoneyChangeType::eMCT_CHECK_ACCOUNT:
		//if (l_i64PlayerMoney  >= cFishApp::m_spControlPanel->GetScoreLimit())
		//{
		//	m_MessageStatus[ePMT_MONEY_LOCKED].m_bMesageOn = true;
		//}
		//else 
		if(eMoneyChangeType::eMCT_CHECK_ACCOUNT == e_eMoneyChangeType)
			m_MessageStatus[ePMT_MONEY_LOCKED].m_bMesageOn = false;
		if ( e_bMessageOn )
		{
			m_MessageStatus[ePMT_PRESS_BUTTON_ADD_SCORE_LIMIT].m_bMesageOn = true;
			m_PressButtonAddScoreLimitMPDIShowTime.Start();
		}
		m_MessageStatus[ePMT_PLEASE_INSERT_COIN].m_bMesageOn = false;
		break;
	/*case eMoneyChangeType::eMCT_LONG_PRESS_BUTTON_ADD_SCORE:
		if (l_i64PlayerMoney >= cFishApp::m_spControlPanel->GetScoreLimit())
		{
			m_MessageStatus[ePMT_MONEY_LOCKED].m_bMesageOn = true;
		}
		if (e_bMessageOn && eMoneyChangeType::eMCT_LONG_PRESS_BUTTON_ADD_SCORE == e_eMoneyChangeType)
		{
			m_MessageStatus[ePMT_PRESS_BUTTON_ADD_SCORE_LIMIT].m_bMesageOn = true;
			m_PressButtonAddScoreLimitMPDIShowTime.Start();
		}
		m_MessageStatus[ePMT_PLEASE_INSERT_COIN].m_bMesageOn = false;
		break;*/
	default:
		break;
	}
	return true;
}

bool	cPlayerMessageBehavior::IsAllowToFire()
{
	//if(	cFishApp::m_spIOControlInterface->IsIOError()||
	if(
		m_bRefundingCoin ||
		//m_MessageStatus[ePMT_MONEY_NOT_ENOUGH].m_bMesageOn ||
		m_MessageStatus[ePMT_MONEY_LOCKED].m_bMesageOn ||
		//m_MessageStatus[ePMT_PRESS_BUTTON_ADD_SCORE_LIMIT].m_bMesageOn ||
		m_MessageStatus[ePMT_PLEASE_INSERT_COIN].m_bMesageOn)
		return false;
	return true;
}

bool	cPlayerMessageBehavior::IsDeleteMessageOn()
{
	if ( m_MessageStatus[ePMT_MONEY_NOT_ENOUGH].m_bMesageOn )
		return true;
	return false;
}
