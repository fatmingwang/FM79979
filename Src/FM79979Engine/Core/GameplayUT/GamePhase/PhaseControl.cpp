#include "PhaseControl.h"
#include "PhasePopupMessager.h"
namespace FATMING_CORE
{
	cPhaseManager*	cSimplePhase::GetPhaseManager()
	{
		if(this->GetOwner())
			return dynamic_cast<cPhaseManager*>(GetOwner());
		return 0;
	}
	//TYPDE_DEFINE_MARCO(cSimplePhase);
 //   void    cSimplePhase::InternalMouseMove(int e_iPosX,int e_iPosY)
	//{
	//	int	l_iCount = this->m_AllMouseBehavior.Count();
	//	for( int i=0;i<l_iCount;++i )
	//		m_AllMouseBehavior[i]->MouseMove(e_iPosX,e_iPosY);
	//}
 //   void    cSimplePhase::InternalMouseDown(int e_iPosX,int e_iPosY)
	//{
	//	int	l_iCount = this->m_AllMouseBehavior.Count();
	//	for( int i=0;i<l_iCount;++i )
	//		m_AllMouseBehavior[i]->MouseMove(e_iPosX,e_iPosY);	
	//}
 //   void    cSimplePhase::InternalMouseUp(int e_iPosX,int e_iPosY)
	//{
	//	int	l_iCount = this->m_AllMouseBehavior.Count();
	//	for( int i=0;i<l_iCount;++i )
	//		m_AllMouseBehavior[i]->MouseMove(e_iPosX,e_iPosY);
	//}
 //   void    cSimplePhase::InternalMouseLeave(int e_iPosX,int e_iPosY)
	//{
	//	int	l_iCount = this->m_AllMouseBehavior.Count();
	//	for( int i=0;i<l_iCount;++i )
	//		m_AllMouseBehavior[i]->MouseMove(e_iPosX,e_iPosY);
	//}
 //   void    cSimplePhase::InternalMouseDoubleClickUp(int e_iPosX,int e_iPosY)
	//{

	//}

	cPhaseManager::cPhaseManager()
	{
		m_iCurrentPhase = -1;
		m_pPhasePopUpMessagerVector = 0;
		m_pCurrentPopUpMessager = 0;
	}

	cPhaseManager::cPhaseManager(cPhaseManager*e_pPhaseManager):cClickMouseBehaviorVector<cSimplePhase>(e_pPhaseManager)
	{
		m_pPhasePopUpMessagerVector = 0;
		m_pCurrentPopUpMessager = 0;
		if( e_pPhaseManager->m_pPhasePopUpMessagerVector )
		{
			m_pPhasePopUpMessagerVector = new cClickMouseBehaviorVector<cPhasePopUpMessager>();
			m_pPhasePopUpMessagerVector->CloneFromList(e_pPhaseManager->m_pPhasePopUpMessagerVector);
		}
	}

	cPhaseManager::~cPhaseManager()
	{
		SAFE_DELETE(m_pPhasePopUpMessagerVector);
	}

	const wchar_t*	cPhaseManager::GetCurrentPhaseName()
	{
		if( this->m_iCurrentPhase == -1)
			return nullptr;
		return	this->m_ObjectList[m_iCurrentPhase]->GetName();
	}

	//char*	cPhaseManager::GetCurrentPhaseNameByChar()
	//{
	//	if( this->m_iCurrentPhase == -1)
	//		return nullptr;
	//	return	UT::WcharToChar(this->m_ObjectList[m_iCurrentPhase]->GetName());
	//}

	bool	cPhaseManager::SetCurrentCurrentPhase(int e_iIndex)
	{
		int	l_i = e_iIndex;
		if (l_i != -1)
		{
			int l_iCount = this->Count();
			m_iCurrentPhase = l_i;
			this->m_pCurrentWorkingObject = this->GetObject(l_i);
			if (m_pCurrentWorkingObject)
			{
				m_pCurrentWorkingObject->Init();
				m_pCurrentWorkingObject->SetSatisfiedCondition(false);
				return true;
			}
			return false;
		}
		m_pCurrentWorkingObject = nullptr;
		return false;
	}

	bool	cPhaseManager::SetCurrentCurrentPhase(const wchar_t*e_strPhaseName)
	{
		int	l_i = this->GetObjectIndexByName(e_strPhaseName);
		return SetCurrentCurrentPhase(l_i);
	}


	bool	cPhaseManager::PopUpMessagerUpdate(float e_fElpaseTime)
	{
		if( m_pCurrentPopUpMessager )
		{
			m_pCurrentPopUpMessager->Update(e_fElpaseTime);
			if( m_pCurrentPopUpMessager->IsSatisfiedCondition() )
			{
				cPhasePopUpMessager::sMessageData*l_pMessageData = m_pCurrentPopUpMessager->GetWorkingMessageData();				
				if( l_pMessageData->strTargetPhaseName.length() > 0 )
				{
					int	l_iNewPhaseIndex = this->GetObjectIndexByName(l_pMessageData->strTargetPhaseName.c_str());
					if( m_iCurrentPhase == l_iNewPhaseIndex )
						SetNewWorkingPhase(l_iNewPhaseIndex,this->m_iCurrentPhase,l_pMessageData->bDestroyOldPhase,l_pMessageData->bInitNewPhase,false);
					else
						SetNewWorkingPhase(l_iNewPhaseIndex,this->m_iCurrentPhase,l_pMessageData->bDestroyOldPhase,l_pMessageData->bInitNewPhase,true);
				}
				m_pCurrentPopUpMessager->Init();
				m_pCurrentPopUpMessager = 0;
				return true;
			}
			return m_pCurrentPopUpMessager->m_bUpdateLastPhase;
		}
		return true;
	}


	void	cPhaseManager::SetNewWorkingPhase(int e_iNewPhaseIndex,int e_iOldPhaseIndex,bool e_bDestroyOldPhase,bool e_bInitNewPhase,bool e_bFetchData)
	{
		cSimplePhase *l_pSimplePhase = m_ObjectList[e_iOldPhaseIndex];
		int	l_iIndex = e_iNewPhaseIndex;
		if( l_iIndex == -1  )
		{
#ifdef WIN32
			FMLog::LogWithFlag(L"error Phase or end phase?? ", CORE_LOG_FLAG);
#endif
		}
		m_iCurrentPhase = l_iIndex;
		cSimplePhase*l_pTargetPhase = 0;
		if( l_iIndex != -1 )
		{
			l_pTargetPhase = m_ObjectList[l_iIndex];
			l_pTargetPhase->SetSatisfiedCondition(false);//if u do not allow m_bSatisfiedCondition to be false again,set it as true in the Init function if u wanted.
			if( e_bFetchData )
				l_pTargetPhase->FetchData(l_pSimplePhase->GetName(),l_pSimplePhase->GetData());
			if( e_bInitNewPhase )
				l_pTargetPhase->Init();
		}
		if( e_bDestroyOldPhase )
			l_pSimplePhase->Destroy();
		if(l_pSimplePhase->IsKillAfterSatisfiedCondition())
			this->RemoveObject(l_pSimplePhase);
		if( l_pTargetPhase )
		{
			m_iCurrentPhase = this->GetObjectIndexByPointer(l_pTargetPhase);
			//try to avoid empty phase render...!?
			//l_pTargetPhase->Update(0.00f);
		}
		this->m_pCurrentWorkingObject = l_pTargetPhase;
		//cGameApp::m_sTimeAndFPS.Update();
	}

	void	cPhaseManager::Update(float	e_fElpaseTime)
	{
		if(PopUpMessagerUpdate(e_fElpaseTime))
		{
			InternalUpdate(e_fElpaseTime);
			this->m_pCurrentWorkingObject = (*this)[m_iCurrentPhase];
			if (m_pCurrentWorkingObject)
			{
				m_pCurrentWorkingObject->Update(e_fElpaseTime);
				if (m_pCurrentWorkingObject->IsSatisfiedCondition())
				{
					int	l_iIndex = GetObjectIndexByName(m_pCurrentWorkingObject->GetNextPhaseName());
					SetNewWorkingPhase(l_iIndex, m_iCurrentPhase);
				}
			}
		}
	}

	void	cPhaseManager::Destroy()
	{
		cClickMouseBehaviorVector<cSimplePhase>::Destroy();
	}

	void	cPhaseManager::DebugRender()
	{
		if(m_pCurrentWorkingObject)
			m_pCurrentWorkingObject->DebugRender();
	}

	bool	cPhaseManager::PopUpMessagerRender()
	{
		if( m_pCurrentPopUpMessager )
		{
			m_pCurrentPopUpMessager->Render();
			return m_pCurrentPopUpMessager->m_bRenderLastPhase;
		}
		return true;
	}

	void	cPhaseManager::Render()
	{
		//if(PopUpMessagerRender())
		{
			InternalRender();
			if(m_pCurrentWorkingObject)
				m_pCurrentWorkingObject->Render();
		}
		PopUpMessagerRender();
	}

	void	cPhaseManager::MouseDown(int e_iX,int e_iY)
	{
		if( m_pCurrentPopUpMessager )
		{
			m_pCurrentPopUpMessager->MouseDown(e_iX,e_iY);
		}
		else
		{
			if( this->m_iCurrentPhase != -1 )
				m_ObjectList[m_iCurrentPhase]->MouseDown(e_iX,e_iY);
		}
	}

	void	cPhaseManager::MouseUp(int e_iX,int e_iY)
	{
		if( m_pCurrentPopUpMessager )
		{
			m_pCurrentPopUpMessager->MouseUp(e_iX,e_iY);
		}
		else
		{
			if( this->m_iCurrentPhase != -1 )
				m_ObjectList[m_iCurrentPhase]->MouseUp(e_iX,e_iY);
		}
	}

	void	cPhaseManager::MouseMove(int e_iX,int e_iY)
	{
		if( m_pCurrentPopUpMessager )
		{
			m_pCurrentPopUpMessager->MouseMove(e_iX,e_iY);
		}
		else
		{
			if( this->m_iCurrentPhase != -1 )
				m_ObjectList[m_iCurrentPhase]->MouseMove(e_iX,e_iY);
		}
	}

	void	cPhaseManager::KeyUp(char e_cKey)
	{
		if( this->m_iCurrentPhase != -1 )
			m_ObjectList[m_iCurrentPhase]->KeyUp(e_cKey);
	}

	void	cPhaseManager::KeyDown(char e_cKey)
	{
		if( this->m_iCurrentPhase != -1 )
			m_ObjectList[m_iCurrentPhase]->KeyDown(e_cKey);
	}

	//void	cPhaseManager::KeyPress(char e_cKey)
	//{
	//	if( this->m_iCurrentPhase != -1 )
	//		m_ObjectList[m_iCurrentPhase]->KeyPress(e_cKey);
	//}

	bool	cPhaseManager::WakePopupMessenger(const wchar_t*e_strName,const wchar_t*e_strPhaseNameName)
	{
		if( m_pPhasePopUpMessagerVector )
		{
			m_pCurrentPopUpMessager = this->m_pPhasePopUpMessagerVector->GetObject(e_strName);
			if( m_pCurrentPopUpMessager )
			{
				m_pCurrentPopUpMessager->Init();
				//m_pCurrentPopUpMessager->SetWakePhaseName(e_strPhaseNameName);
				return true;
			}
		}
		return false;
	}
	//<Root>
	//	<cPhasePopUpMessager Name="" Init="" RenderLastPhase="" UpdateLastPhase="">
	//		<Messenger Name="" >
	//			<image butto or cAssembleImageButton>
	//		<Messenger/>
	//	</cPhasePopUpMessager>
	//	<cPhasePopUpMessager Name="">
	//		<Messenger Name="" >
	//			<image butto or cAssembleImageButton>
	//		<Messenger/>
	//	</cPhasePopUpMessager>
	//	<cPhasePopUpMessager Name="">
	//		<Messenger Name="" >
	//			<image butto or cAssembleImageButton>
	//		<Messenger/>
	//	</cPhasePopUpMessager>
	//</Root>
	void	cPhaseManager::RegisterPopupMessenger(char*e_strFileName)
	{
		cNodeISAX	l_cNodeISAX;
		if(l_cNodeISAX.ParseDataIntoXMLNode(e_strFileName))
		{
			TiXmlElement*l_pElement = l_cNodeISAX.GetRootElement();
			if( l_pElement )
			{
				l_pElement = l_pElement->FirstChildElement();
				while( l_pElement )
				{
					cPhasePopUpMessager*l_pPhasePopUpMessager = cPhasePopUpMessager::GetMe(l_pElement);
					if( l_pPhasePopUpMessager )
					{
						if( !m_pPhasePopUpMessagerVector )
							m_pPhasePopUpMessagerVector = new cClickMouseBehaviorVector<cPhasePopUpMessager>();
						if(!m_pPhasePopUpMessagerVector->AddObject(l_pPhasePopUpMessager))
						{
							UT::ErrorMsg(l_pPhasePopUpMessager->GetName(),L"cPhasePopUpMessager name exists cPhaseManager::RegisterPopupMessenger");
						}
					}
					else
						UT::ErrorMsg(l_pPhasePopUpMessager->GetName(),L"cPhasePopUpMessager name exists cPhaseManager::RegisterPopupMessenger");
					l_pElement = l_pElement->NextSiblingElement();
				}
			}
		}
		else
		{
			UT::ErrorMsg(e_strFileName,"parse failed cPhaseManager::RegisterPopupMessenger");
		}
	}

	cPhasePopUpMessager*	cPhaseManager::RegisterPopupMessenger(TiXmlElement*e_pTiXmlElement)
	{
		cPhasePopUpMessager*l_pPhasePopUpMessager = cPhasePopUpMessager::GetMe(e_pTiXmlElement);
		if( !l_pPhasePopUpMessager )
			return 0;
		if( !m_pPhasePopUpMessagerVector )
			m_pPhasePopUpMessagerVector = new cClickMouseBehaviorVector<cPhasePopUpMessager>();	
		if(!m_pPhasePopUpMessagerVector->AddObject(l_pPhasePopUpMessager))
		{
			SAFE_DELETE(l_pPhasePopUpMessager);
			UT::ErrorMsg(l_pPhasePopUpMessager->GetName(),L"cPhasePopUpMessager name exists cPhaseManager::RegisterPopupMessenger");
			return 0;
		}
		return l_pPhasePopUpMessager;
	}
//end namespace
}