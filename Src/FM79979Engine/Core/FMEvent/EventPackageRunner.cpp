#include "../stdafx.h"
#include "../AllCoreInclude.h"
#include "../Particle/AllParticleInclude.h"
#include "EventManager.h"
#include "EventPackageRunner.h"

namespace   FATMING_CORE
{
    //=========================================
    //
    //=========================================    

	cEventPackage::cEventPackage()
	{
		m_vBGColor = Vector4(1.f,0.f,0.f,1.f);
		m_vPagePos = Vector3::Zero;
		m_bRenderBGColor = false;
	}
	cEventPackage::cEventPackage(cEventPackage*e_pEventPackage):cDoAllMouseBehaviorList<cEventObjectInstance>(e_pEventPackage)
    {
        m_vPagePos = e_pEventPackage->m_vPagePos;
        m_vBGColor = e_pEventPackage->m_vBGColor;
    }

    void    cEventPackage::SetRelativePosition( Vector3 e_vPos )
    {
        m_vPagePos = e_vPos;
    }

    void    cEventPackage::Init()
    {
        LIST_DO_FOR_ALL_CHILDREN(this,Init());
    }

    void    cEventPackage::Destroy()
    {
        LIST_DO_FOR_ALL_CHILDREN(this,Destroy());
    }

    void    cEventPackage::Update(float e_fElpaseTime)
    {
		LIST_DO_FOR_ALL_CHILDREN(this,Update(e_fElpaseTime));
    }

	void    cEventPackage::Render()
	{
		if( m_bRenderBGColor )
		{
			glClearColor( this->m_vBGColor.x,this->m_vBGColor.y,this->m_vBGColor.z,this->m_vBGColor.w );
		}
        //m_vPagePos = e_pEventPackage->m_vPagePos;
        //m_vBGColor = e_pEventPackage->m_vBGColor;
		LIST_DO_FOR_ALL_CHILDREN(this,Render());
	}

	void	cEventPackage::SetBGColor(Vector4 e_vColor)
	{
		m_vBGColor = e_vColor;
		m_bRenderBGColor = true;
	}
    //=========================================
    //
    //=========================================    
	cEventPackageRunner::cEventPackageRunner(cEventManager*e_pEventManager)
	{
		m_pEventManager = e_pEventManager;
		m_pCurrentEventPackage = 0;
		//m_vBGColor = Vector4(0.f,0.f,0.f,1.f);
		//m_vPagePos = Vector3::Zero;
	}

	cEventPackageRunner::~cEventPackageRunner()
	{
		if( m_pCurrentEventPackage )
		{
			cEventPackage*l_pTemp = new cEventPackage();
			RemoveNotOverlapObject(m_pCurrentEventPackage,l_pTemp);
			SAFE_DELETE(m_pCurrentEventPackage);
			delete l_pTemp;
		}
	}

	void	cEventPackageRunner::Init()
	{
		g_pCurrentEventManager = this->m_pEventManager;
		const WCHAR*l_strChangePachageName = m_pEventManager->m_pstrChangePachageName->c_str();
		if( wcslen(l_strChangePachageName) )
		{
			cEventVariable*l_pNameAndData = m_pEventManager->m_pEventVariableManager->GetObject(l_strChangePachageName);
			m_strNewPageName = *l_pNameAndData->GetWString();
		}
	}

    void    cEventPackageRunner::Update(float e_fElpaseTime)
    {
		g_pCurrentEventManager = this->m_pEventManager;
        if(m_pCurrentEventPackage)
		{
			if( m_pCurrentEventPackage )
				m_pCurrentEventPackage->Update(e_fElpaseTime);
			const WCHAR*l_strChangePachageName = m_pEventManager->m_pstrChangePachageName->c_str();
			cEventVariable*l_pNameAndData = m_pEventManager->m_pEventVariableManager->GetObject(l_strChangePachageName);
			if( l_pNameAndData )
			{
				const WCHAR*l_strNewPackageName = l_pNameAndData->GetWString()->c_str();
				if( wcscmp(m_strNewPageName.c_str(),l_strNewPackageName) )
				{
					LoadPackage(l_pNameAndData->GetWString()->c_str());
				}
			}
		}
    }

	void	cEventPackageRunner::Render()
	{
		g_pCurrentEventManager = this->m_pEventManager;
        if(m_pCurrentEventPackage)
		{
			m_pCurrentEventPackage->Render();
		}	
	}

    void    cEventPackageRunner::MouseMove(int e_iPosX,int e_iPosY)
	{
		g_pCurrentEventManager = this->m_pEventManager;
		if( m_pCurrentEventPackage )
			m_pCurrentEventPackage->MouseMove(e_iPosX,e_iPosY);
	}

    void    cEventPackageRunner::MouseDown(int e_iPosX,int e_iPosY)
	{
		g_pCurrentEventManager = this->m_pEventManager;
		if( m_pCurrentEventPackage )
			m_pCurrentEventPackage->MouseDown(e_iPosX,e_iPosY);
	}

    void    cEventPackageRunner::MouseUp(int e_iPosX,int e_iPosY)
	{
		g_pCurrentEventManager = this->m_pEventManager;
		if( m_pCurrentEventPackage )
			m_pCurrentEventPackage->MouseUp(e_iPosX,e_iPosY);
	}

	void	cEventPackageRunner::RemoveUnUsingEventObjectStatusAtEventManager(cEventPackage*e_pOldEventPackage,cEventPackage*e_pNewEventPackage)
	{
		int	l_iNewEventPackageCount = e_pNewEventPackage->Count();
		int	l_iOldEventPackageCount = e_pOldEventPackage->Count();
		for( int i=0;i<l_iOldEventPackageCount;++i )
		{
			bool	l_bSame = false;
			cEventObjectInstance*l_pOldEventObjectInstance = e_pOldEventPackage->GetObject(i);
			cEventObjectStatus*l_pEventObjectStatus = l_pOldEventObjectInstance->GetEventObjectStatus();
			for( int j=0;j<l_iNewEventPackageCount;++j )
			{
				cEventObjectInstance*l_pNewEventObjectInstance = e_pNewEventPackage->GetObject(j);
				if(l_pNewEventObjectInstance->GetEventObjectStatus() == l_pEventObjectStatus)
				{
					l_bSame = true;
					break;
				}
			}
			//remove from
			if( !l_bSame )
			{
				if(l_pEventObjectStatus)
				{
					if( m_pEventManager->m_pEventObjectStatusList )//somehow delete sort problem?
						m_pEventManager->m_pEventObjectStatusList->RemoveObject(l_pEventObjectStatus);
				}
			}
		}
	}

	void	cEventPackageRunner::RemoveUnUsingEventInstanceAtEventManager(cEventPackage*e_pOldEventPackage,cEventPackage*e_pNewEventPackage)
	{
		//1st collet old list
		//2nd remove old list's data if new is exist 
		//3rd remove the same data at g_pCurrentEventManager.
		cNamedTypedObjectVector<cEventInstance>	l_EventInstanceList;
		int	l_iNewEventPackageCount = e_pNewEventPackage->Count();
		int	l_iOldEventPackageCount = e_pOldEventPackage->Count();
		for( int i=0;i<l_iOldEventPackageCount;++i )
		{
			cEventObjectInstance*l_pOldEventObjectInstance = e_pOldEventPackage->GetObject(i);
			l_EventInstanceList.AddListToMe(l_pOldEventObjectInstance->GetEventInstanceList());
		}
		for( int i=0;i<l_iNewEventPackageCount;++i )
		{
			cEventObjectInstance*l_pNewEventObjectInstance = e_pNewEventPackage->GetObject(i);
			cClickMouseBehaviorVector<cEventInstance>*l_pEventInstanceList = l_pNewEventObjectInstance->GetEventInstanceList();
			int	l_iEventInstanceCount = l_pEventInstanceList->Count();
			for( int j=0;j<l_iEventInstanceCount;++j )
			{
				const WCHAR*l_strName = l_pEventInstanceList->GetObject(j)->GetName();
				l_EventInstanceList.RemoveObjectWithoutDelete(l_strName);
			}
		}
		int	l_iCount = l_EventInstanceList.Count();
		for( int i=0;i<l_iCount;++i )
		{
			const WCHAR*l_strName = l_EventInstanceList.GetObject(i)->GetName();
			if( m_pEventManager->m_pEventInstanceList )//somehow delete sort problem?
			{
				int	l_iIndex = m_pEventManager->m_pEventInstanceList->GetObjectIndexByName(l_strName);
				assert(l_iIndex != -1&&"this event instance not in the m_pEventManager...");
				m_pEventManager->m_pEventInstanceList->RemoveObject(l_iIndex);
			}
		}
	}

	void	cEventPackageRunner::StatusResourceDataChangeWhilePackageChange(NamedTypedObject*e_pNamedTypedObject)
	{
		if( e_pNamedTypedObject->Type() == cOpanalWAV::TypeID || e_pNamedTypedObject->Type() == cOpanalOgg::TypeID  )
		{
			cBasicSound*l_pBasicSound = (cBasicSound*)e_pNamedTypedObject;
			if(l_pBasicSound->IsStopWhileLeave())
			{
				l_pBasicSound->Play(false);
			}
		}
	}


	void	cEventPackageRunner::RemoveUnUsingResourceAtEventManager(cEventPackage*e_pOldEventPackage,cEventPackage*e_pNewEventPackage)
	{
		cNamedTypedObjectVector<NamedTypedObject>	l_OldResourceList;
		cNamedTypedObjectVector<NamedTypedObject>	l_NewResourceList;
		cNamedTypedObjectVector<NamedTypedObject>	l_ReleaseResourceList;
		l_OldResourceList.SetFromResource(true);
		l_NewResourceList.SetFromResource(true);
		l_ReleaseResourceList.SetFromResource(true);
		int	l_iNewEventPackageCount = e_pNewEventPackage->Count();
		int	l_iOldEventPackageCount = e_pOldEventPackage->Count();
		for( int i=0;i<l_iOldEventPackageCount;++i )
		{
			cEventObjectInstance*l_pOldEventObjectInstance = e_pOldEventPackage->GetObject(i);
			l_pOldEventObjectInstance->CollectResourceList(&l_OldResourceList);
		}
		for( int i=0;i<l_iNewEventPackageCount;++i )
		{
			cEventObjectInstance*l_pNewEventObjectInstance = e_pNewEventPackage->GetObject(i);
			l_pNewEventObjectInstance->CollectResourceList(&l_NewResourceList);
		}
		//now compare if old data is not exist at new data sotre and release it.
		int	l_iCount = l_OldResourceList.Count();
		for( int i=0;i<l_iCount;++i )
		{
			NamedTypedObject*l_pRecource = l_OldResourceList.GetObject(i);
			//do something for 
			StatusResourceDataChangeWhilePackageChange(l_pRecource);
			int	l_iIndex = l_NewResourceList.GetObjectIndexByName(l_OldResourceList.GetObject(i)->GetName());
			if( l_iIndex == -1 )
			{
				l_ReleaseResourceList.AddObject(l_pRecource);
			}
		}
		//now check MPDIList if there are some resource still using PI ,MPDIList remove pi so pi will be keep.
		l_iCount = l_ReleaseResourceList.Count();
		for( int i=0;i<l_iCount;++i )
		{
			if( l_ReleaseResourceList[i]->Type() == cMPDIList::TypeID )
			{
				cMPDIList*l_pMPDIList = dynamic_cast<cMPDIList*>(l_ReleaseResourceList[i]);
				int	l_iNewResourceListCount = l_NewResourceList.Count();
				for( int j=0;j<l_iNewResourceListCount;++j )
				{
					if( l_NewResourceList[j]->Type() == cPuzzleImage::TypeID )
					{
						const WCHAR*l_strPIName = l_NewResourceList[j]->GetName();
						if(l_pMPDIList->GetPuzzleImage( l_strPIName ))
						{//there are new object using this pi so do not force to remove pi reference to avoid cGameApp::m_spAnimationParser to delete this pi
							l_pMPDIList->ForceRemovePIReference(dynamic_cast<cPuzzleImage*>(l_NewResourceList[j]));
						}
					}
				}
			}
			else
			if( l_ReleaseResourceList[i]->Type() == cOpanalWAV::TypeID || l_ReleaseResourceList[i]->Type() == cOpanalOgg::TypeID )
			{
				cBasicSound*l_pBasicSound = (cBasicSound*)l_ReleaseResourceList[i];
				l_pBasicSound->Play(false);
				//cGameApp::m_spSoundParser->RemoveObject(l_pBasicSound);
			}
			else
			{
				UT::ErrorMsg(l_ReleaseResourceList[i]->Type(),L"please release ressource");
			}
		}
		//IntelliSense
		l_iCount = l_ReleaseResourceList.Count();
		for( int i=0;i<l_iCount;++i )
		{
			m_pEventManager->RemoveResource(l_ReleaseResourceList[i]);
		}
		
	}

	void	cEventPackageRunner::RemoveNotOverlapObject(cEventPackage*e_pOldEventPackage,cEventPackage*e_pNewEventPackage)
	{
		if( !e_pOldEventPackage )
			return;
		RemoveUnUsingEventObjectStatusAtEventManager(e_pOldEventPackage,e_pNewEventPackage);
		RemoveUnUsingEventInstanceAtEventManager(e_pOldEventPackage,e_pNewEventPackage);
		RemoveUnUsingResourceAtEventManager(e_pOldEventPackage,e_pNewEventPackage);
	}

	cEventPackage*			cEventPackageRunner::GetCurrentEventPackage(){return m_pCurrentEventPackage;}
	void					cEventPackageRunner::SetCurrentEventPackage(cEventPackage*e_pEventPackage){ m_pCurrentEventPackage = e_pEventPackage; }

	bool	cEventPackageRunner::LoadPackage(const WCHAR*e_strPackageName)
	{
		g_pCurrentEventManager = this->m_pEventManager;
		m_strNewPageName = e_strPackageName;
		TiXmlElement*l_pTiXmlElement = this->m_pEventManager->GetChildOfRootXmlElementByAttribueName(L"EventPackage",e_strPackageName,NAME);
		cEventPackage*l_pCurrentEventPackage = 0;
		if( !l_pTiXmlElement )
		{
			l_pCurrentEventPackage = m_pEventManager->GetEventPackage(e_strPackageName);
		}
		if( l_pTiXmlElement || l_pCurrentEventPackage )
		{
			cGameApp::m_siShowErrorType = 0;
			//force to remove all partivle resource,because I am lazy to calculate relatived in use or not
			this->m_pEventManager->m_pPrtGList->Destroy();
			if( !l_pCurrentEventPackage )
				l_pCurrentEventPackage = this->m_pEventManager->GetEventPackage(l_pTiXmlElement);
			RemoveNotOverlapObject(m_pCurrentEventPackage,l_pCurrentEventPackage);
			if( m_pCurrentEventPackage )
			{//external class do not killt it.
				if( m_pCurrentEventPackage->GetOwner() != m_pEventManager->m_pExternalEventPackage )
				{
					SAFE_DELETE(m_pCurrentEventPackage);
				}
			}
			m_pCurrentEventPackage = l_pCurrentEventPackage;
			if( m_pCurrentEventPackage )
				m_pCurrentEventPackage->Init();
			cGameApp::m_siShowErrorType = 1;
			if(wcslen(cGameApp::m_spstrErrorMsgString->c_str()))
			{
				UT::ErrorMsg(cGameApp::m_spstrErrorMsgString->c_str(),e_strPackageName);
			}
			cGameApp::m_spstrErrorMsgString->clear();
			const WCHAR*l_strChangePachageName = m_pEventManager->m_pstrChangePachageName->c_str();
			cEventVariable*l_pNameAndData = m_pEventManager->m_pEventVariableManager->GetObject(l_strChangePachageName);
			if( l_pNameAndData )
			{
				l_pNameAndData->SetDataByString(e_strPackageName);
			}
			return true;
		}
		else
		{//leave event!?
			if( m_pCurrentEventPackage )
			{
				if( m_pCurrentEventPackage->GetOwner() != m_pEventManager->m_pExternalEventPackage )
				{
					SAFE_DELETE(m_pCurrentEventPackage);
				}
			}
		}
		return false;
	}

	bool	cEventPackageRunner::LoadPackage()
	{
		const WCHAR*l_strChangePachageName = m_pEventManager->m_pstrChangePachageName->c_str();
		cEventVariable*l_pNameAndData = m_pEventManager->m_pEventVariableManager->GetObject(l_strChangePachageName);
		if( !l_pNameAndData )
		{
			return false;
		}
		g_pCurrentEventManager = this->m_pEventManager;
		m_strNewPageName = l_pNameAndData->ConvertToString();
		TiXmlElement*l_pTiXmlElement = this->m_pEventManager->GetChildOfRootXmlElementByAttribueName(L"EventPackage",m_strNewPageName.c_str(),NAME);
		if( l_pTiXmlElement )
		{
			cGameApp::m_siShowErrorType = 0;
			//force to remove all partivle resource,because I am lazy to calculate relatived in use or not
			this->m_pEventManager->m_pPrtGList->Destroy();
			cEventPackage*l_pCurrentEventPackage = this->m_pEventManager->GetEventPackage(l_pTiXmlElement);
			RemoveNotOverlapObject(m_pCurrentEventPackage,l_pCurrentEventPackage);
			if( m_pCurrentEventPackage )
			{
				delete m_pCurrentEventPackage;
			}
			m_pCurrentEventPackage = l_pCurrentEventPackage;
			if( m_pCurrentEventPackage )
				m_pCurrentEventPackage->Init();
			cGameApp::m_siShowErrorType = 1;
			if(wcslen(cGameApp::m_spstrErrorMsgString->c_str()))
			{
				UT::ErrorMsg(cGameApp::m_spstrErrorMsgString->c_str(),m_strNewPageName.c_str());
			}
			cGameApp::m_spstrErrorMsgString->clear();
			return true;
		}
		else
		{//leave event!?
			SAFE_DELETE(m_pCurrentEventPackage);
		}
		return false;	
	}

	const WCHAR*	cEventPackageRunner::GetNextPhaseName()
	{
		return 0;
	}
//end namespace   FATMING_CORE
}