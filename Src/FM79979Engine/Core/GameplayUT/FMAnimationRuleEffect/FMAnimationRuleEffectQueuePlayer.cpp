#include "../stdafx.h"
#include "FMAnimationRuleEffectQueuePlayer.h"
#include "../Particle/AllParticleInclude.h"
#include "GameApp.h"

namespace FATMING_CORE
{
	//<cFMARQueuePlayer>
	//	<ObjectNameAndTime Time="" Type="MPDI" MPDIList="aaxx.mpdi" MPDI="">
	//		<Data MPDIList="aaxx.mpdi" MPDI=""/>
	//		<ObjectNameAndTime Time="" Type="MPDI" />
	//			<Data MPDIList="aaxx.mpdi" MPDI=""/>
	//		</ObjectNameAndTime>
	//	</ObjectNameAndTime>
	//	<ObjectNameAndTime Time="" Type="Particle" PrtFile="aaxx.prt" Emitter="" />
	//</cFMARQueuePlayer>

	TYPDE_DEFINE_MARCO(cFMARQueuePlayer);
	cFMARQueuePlayer::cFMARQueuePlayer(TiXmlElement*e_pElement)
	{
		TiXmlElement*l_pElement = e_pElement;
		m_pFMAnimationRule = 0;
		ELEMENT_VALUE_ASSERT_CHECK(l_pElement,cFMARQueuePlayer::TypeID);
		float	l_fEndTime = 0.f;
		eFMAnimationRuleType	l_eFMAnimationRuleType = eFM_MAX;
		PARSE_ELEMENT_START(l_pElement)
			COMPARE_NAME("Type")
			{
				l_eFMAnimationRuleType = GetFMAnimationRuleType(l_strValue);
			}
			else
			COMPARE_NAME("StartTime")
			{
				this->m_fStartTime = GetFloat(l_strValue);
			}
			else
			COMPARE_NAME("Name")
			{
				this->SetName(l_strValue);
			}
		PARSE_NAME_VALUE_END
		l_pElement = l_pElement->FirstChildElement();
		if( l_pElement )
		{
			const wchar_t*l_strValue = l_pElement->Value();
			COMPARE_VALUE("Data")
			{
				if( l_eFMAnimationRuleType == eFM_MPDI )
				{
					ProcssMPDIEffect(l_pElement);
				}
				else
				if( l_eFMAnimationRuleType == eFM_PARTICLE )
				{
					ProcssParticleEffect(l_pElement);
				}
			}
		}
		if( m_pFMAnimationRule )
		{
			l_fEndTime = this->m_pFMAnimationRule->GetEndTime();
			m_fTotalPlayTime = l_fEndTime;
		}
		l_pElement = l_pElement->NextSiblingElement();
		while( l_pElement )
		{
			const wchar_t*l_strValue = l_pElement->Value();
			COMPARE_VALUE_WITH_DEFINE(cFMARQueuePlayer::TypeID)
			{
				cFMARQueuePlayer*l_pFMARQueuePlayer = new cFMARQueuePlayer(l_pElement);
				//some cpu will get a epsional time offset but it should't happen
				bool	l_bPlaySequence = false;
				if( l_pFMARQueuePlayer->GetStartTime() < 0 )
				{
					l_pFMARQueuePlayer->SetStartTime(m_fTotalPlayTime);
					l_bPlaySequence  =true;
				}
				l_pFMARQueuePlayer->SetTotalPlayTime(l_pFMARQueuePlayer->GetEndTime()+l_pFMARQueuePlayer->GetStartTime(),l_pFMARQueuePlayer->GetStartTime());
				//FMLog::LogWithFlag(L"\t");
				//FMLog::LogWithFlag(l_pFMARQueuePlayer->m_pFMAnimationRule->GetName());
				//FMLog::LogWithFlag(L"___Start:");
				//FMLog::LogWithFlag(ValueToStringW(l_pFMARQueuePlayer->m_fStartTime).c_str());
				//FMLog::LogWithFlag(L"___Total:");
				//FMLog::LogWithFlag(ValueToStringW(l_pFMARQueuePlayer->m_fTotalPlayTime).c_str());
				//FMLog::LogWithFlag(L"\n");
				this->AddObjectNeglectExist(l_pFMARQueuePlayer);
				float	l_fChildEndTime = l_pFMARQueuePlayer->GetEndTime();
				if( l_bPlaySequence )
				{
					if( m_pFMAnimationRule && m_pFMAnimationRule->GetEndTime() < l_pFMARQueuePlayer->GetEndTime() )
					{
						l_fEndTime = l_fChildEndTime;
					}
					else
					if( l_fEndTime < l_fChildEndTime )
						l_fEndTime = l_fChildEndTime;
				}
				else
				if( l_fEndTime < l_fChildEndTime )
					l_fEndTime = l_fChildEndTime;
			}
			l_pElement = l_pElement->NextSiblingElement();
		}
		this->m_fTotalPlayTime = l_fEndTime;
		FMLog::LogWithFlag(m_pFMAnimationRule->GetName(), CORE_LOG_FLAG);
		FMLog::LogWithFlag(L"___Start:", CORE_LOG_FLAG);
		FMLog::LogWithFlag(ValueToStringW(this->m_fStartTime).c_str(), CORE_LOG_FLAG);
		FMLog::LogWithFlag(L"___Total:", CORE_LOG_FLAG);
		FMLog::LogWithFlag(ValueToStringW(m_fTotalPlayTime).c_str(), CORE_LOG_FLAG);
		FMLog::LogWithFlag(L"\n", CORE_LOG_FLAG);
	}

	
	cFMARQueuePlayer::cFMARQueuePlayer(cFMARQueuePlayer*e_pFARQueuePlayer):cFatmingGroupBehaviorList<cFMTimeLineAnimationRule>(e_pFARQueuePlayer)
	{
		m_pFMAnimationRule = 0;
		if( e_pFARQueuePlayer->m_pFMAnimationRule )
		{
			NamedTypedObject*l_pNamedTypedObject = e_pFARQueuePlayer->m_pFMAnimationRule->Clone();
			m_pFMAnimationRule = dynamic_cast<cFMTimeLineAnimationRule*>(l_pNamedTypedObject);
			
		}
	}

	cFMARQueuePlayer::~cFMARQueuePlayer()
	{
		SAFE_DELETE(m_pFMAnimationRule);
	}
	//			<Data MPDIList="aaxx.mpdi" MPDI=""/>
	void	cFMARQueuePlayer::ProcssMPDIEffect(TiXmlElement*e_pElement)
	{
		cMPDIList*l_pMPDIList = 0;
		cMPDI*l_pMPDI = 0;
		PARSE_ELEMENT_START(e_pElement)
			COMPARE_NAME("FileName")
			{
				l_pMPDIList = cGameApp::GetMPDIListByFileName(l_strValue,true);
			}
			else
			COMPARE_NAME("MPDI")
			{
				if( l_pMPDIList )
					l_pMPDI = l_pMPDIList->GetObject(l_strValue);
			}
		PARSE_NAME_VALUE_END
		if( l_pMPDI )
			m_pFMAnimationRule = dynamic_cast<cMPDI*>(l_pMPDI->Clone());
		
	}

	void	cFMARQueuePlayer::ProcssParticleEffect(TiXmlElement*e_pElement)
	{
		cFMTimeLineAnimationRule*l_pFMAnimationRule = 0;
		std::wstring	l_strFileName;
		PARSE_ELEMENT_START(e_pElement)
			COMPARE_NAME("FileName")
			{
				l_strFileName = l_strValue;
			}
			else
			COMPARE_NAME("PRTG")
			{
				l_pFMAnimationRule = cGameApp::GetPrtEmitter(l_strFileName.c_str(),l_strValue);
			}
			else
			COMPARE_NAME("Emitter")
			{
				m_pFMAnimationRule = cGameApp::GetPRTG(l_strFileName.c_str(),l_strValue);
			}
		PARSE_NAME_VALUE_END
		if( l_pFMAnimationRule )
			m_pFMAnimationRule = dynamic_cast<cFMTimeLineAnimationRule*>(l_pFMAnimationRule->Clone());
						
	}

    void	cFMARQueuePlayer::InternalInit()
	{
		if( m_pFMAnimationRule )
			m_pFMAnimationRule->Init();
		cFatmingGroupBehaviorList<cFMTimeLineAnimationRule>::InternalInit();
	}

    void	cFMARQueuePlayer::InternalUpdate(float e_fElpaseTime)
	{
		if( m_pFMAnimationRule )
			m_pFMAnimationRule->Update(e_fElpaseTime);
		LIST_DO_FOR_ALL_CHILDREN(this,Update(e_fElpaseTime));
	}

    void	cFMARQueuePlayer::InternalRender()
	{
		if( m_pFMAnimationRule )
			m_pFMAnimationRule->Render();
		//cFatmingGroupBehaviorList<cFMTimeLineAnimationRule>::Render();
		LIST_DO_FOR_ALL_CHILDREN(this,Render());
		if( this->Count() == 1  )
		{
			cFMARQueuePlayer*l_pFMARQueuePlayer = dynamic_cast<cFMARQueuePlayer*>(this->GetObject(0));
			if( l_pFMARQueuePlayer->Count() == 1 )
				cGameApp::RenderFont(100,100,ValueToStringW(this->m_fPastTime).c_str());
			else
				cGameApp::RenderFont(100,200,ValueToStringW(this->m_fPastTime).c_str());
		}
		if( this->Count() == 0 )
		{
			cGameApp::RenderFont(100,300,ValueToStringW(this->m_fPastTime).c_str());
		}
	}

	void	cFMARQueuePlayer::SetData(void*e_pData)
	{
		Vector3	l_vPos = *(Vector3*)e_pData;
		if( this->m_pFMAnimationRule )
			m_pFMAnimationRule->SetPos(l_vPos);
		cFatmingGroupBehaviorList<cFMTimeLineAnimationRule>::SetData(e_pData);
	}

	void	cFMARQueuePlayer::RearrangeTime(float e_fNewTime){ assert(0&&"not support"); }
	void	cFMARQueuePlayer::RearrangeTimeByPercent(float e_fPercenttage){ assert(0&&"not support"); }
	void	cFMARQueuePlayer::RenderByGlobalTime(float e_fTime){ assert(0&&"not support"); }
	void	cFMARQueuePlayer::InvertOrder(){ assert(0&&"not support"); }
//end namespace FATMING_CORE
}