#include "MultiSelectOptionByMPDI.h"
#include "../../GameApp/GameApp.h"
namespace FATMING_CORE
{
	TYPDE_DEFINE_MARCO(cMultiSelectOptionByMPDI);
	TYPDE_DEFINE_MARCO(cSelectOptionByCueToStartCurveWithTime);
    
    cSelectOptionByCueToStartCurveWithTime::cSelectOptionByCueToStartCurveWithTime(cMPDIList*e_pMPDIList,int e_iIndex,wchar_t*e_strDescription)
    {
        if( e_strDescription )
            m_strDescription = e_strDescription;
        if( e_pMPDIList )
        {
            wchar_t*l_ppNameRule[] = {L"Idle",L"Horver",L"Selected"};
            cMPDI*l_pIdle = e_pMPDIList->GetObject(l_ppNameRule[0]);
            cMPDI*l_pHorver = e_pMPDIList->GetObject(l_ppNameRule[1]);
            cMPDI*l_pSelected = e_pMPDIList->GetObject(l_ppNameRule[2]);
			AddDefaultObject(e_pMPDIList->GetObject(l_ppNameRule[0])->GetObject(e_iIndex),e_pMPDIList->GetObject(l_ppNameRule[1])->GetObject(e_iIndex),e_pMPDIList->GetObject(l_ppNameRule[2])->GetObject(e_iIndex));
        }
    }
	cSelectOptionByCueToStartCurveWithTime::cSelectOptionByCueToStartCurveWithTime(cCueToStartCurveWithTime*e_pIdel,cCueToStartCurveWithTime*e_pHorver,cCueToStartCurveWithTime*e_pSelect,wchar_t*e_strDescription)
	{
        if( e_strDescription )
            m_strDescription = e_strDescription;
        AddDefaultObject(e_pIdel,e_pHorver,e_pSelect);
	}
    //
    cSelectOptionByCueToStartCurveWithTime::cSelectOptionByCueToStartCurveWithTime(cCueToStartCurveWithTime*e_pIdel,cCueToStartCurveWithTime*e_pHorver,cCueToStartCurveWithTime*e_pSelect,Vector3 e_vPos,int e_iPIImageIndex)
    {
		AddDefaultObject(e_pIdel,e_pHorver,e_pSelect);
        for( int i=0;i<eSOS_MAX;++i )
        {
			this->m_ObjectList[i]->SetPos(e_vPos);
            m_ObjectList[i]->ChangeAllImageByImageIndex(e_iPIImageIndex);
        }
    }
    //
    cSelectOptionByCueToStartCurveWithTime::~cSelectOptionByCueToStartCurveWithTime()
    {

    }

	void	cSelectOptionByCueToStartCurveWithTime::AddDefaultObject(cCueToStartCurveWithTime*e_pIdel,cCueToStartCurveWithTime*e_pHorver,cCueToStartCurveWithTime*e_pSelect)
	{
		this->Destroy();
        cCueToStartCurveWithTime*l_peSOS_IDLE = new cCueToStartCurveWithTime(e_pIdel);
        cCueToStartCurveWithTime*l_peSOS_HORVER = new cCueToStartCurveWithTime(e_pHorver);
        cCueToStartCurveWithTime*l_peSOS_SELECTED = new cCueToStartCurveWithTime(e_pSelect);
		l_peSOS_IDLE->SetAnimationLoop(true);
		l_peSOS_HORVER->SetAnimationLoop(true);
		this->AddObjectNeglectExist(l_peSOS_IDLE);
		this->AddObjectNeglectExist(l_peSOS_HORVER);
		this->AddObjectNeglectExist(l_peSOS_SELECTED);
        //not true is because we want to skip this phase
        //m_ObjectList[eSOS_SELECTED]->SetAnimationLoop(true);
        //all cCueToStartCurveWithTime name should be same!.
        this->SetName(l_peSOS_SELECTED->GetName());
	}

	cSelectOptionByCueToStartCurveWithTime::cSelectOptionByCueToStartCurveWithTime(cSelectOptionByCueToStartCurveWithTime*e_pSelectOptionByCueToStartCurveWithTime)
	{
		CloneFromList(e_pSelectOptionByCueToStartCurveWithTime);
		m_strDescription = e_pSelectOptionByCueToStartCurveWithTime->m_strDescription;
		this->SetName(e_pSelectOptionByCueToStartCurveWithTime->GetObject(eSOS_SELECTED)->GetName());
	}
    ////for state change
    void    cSelectOptionByCueToStartCurveWithTime::InternalMouseMove(int e_iPosX,int e_iPosY)
    {
		if( this->m_ObjectList[eSOS_SELECTED] != this->m_pCurrentWorkingObject )
			this->SetCurrentWorkingObject(eSOS_HORVER);
    }
    //first time mouse into
    void    cSelectOptionByCueToStartCurveWithTime::InternalMouseDown(int e_iPosX,int e_iPosY)
    {
        if( this->m_ObjectList[eSOS_SELECTED] != this->m_pCurrentWorkingObject )
            this->SetCurrentWorkingObject(eSOS_HORVER);
    }
    
    //while mouse clicked is up
    void    cSelectOptionByCueToStartCurveWithTime::InternalMouseUp(int e_iPosX,int e_iPosY)
    {
        if( this->m_ObjectList[eSOS_SELECTED] != this->m_pCurrentWorkingObject )
            this->SetCurrentWorkingObject(eSOS_SELECTED);
    }
 
    void    cSelectOptionByCueToStartCurveWithTime::InternalMouseLeave(int e_iPosX,int e_iPosY)
    {
        this->SetCurrentWorkingObject(eSOS_IDLE);
    }
    
    bool    cSelectOptionByCueToStartCurveWithTime::InternalCollide(int e_iPosX,int e_iPosY)
    {
        if(this->m_pCurrentWorkingObject->Collide(e_iPosX,e_iPosY))
        {
			m_bCollided = true;
        }
		else
			m_bCollided = false;
        return m_bCollided;
    }
    
    void    cSelectOptionByCueToStartCurveWithTime::Render()
    {
        if( m_pCurrentWorkingObject->IsAnimationDone() )
            m_pCurrentWorkingObject->RenderLastFrame();
        else
            this->m_pCurrentWorkingObject->Render();
        if( m_strDescription.length() )
        {
            Vector3 l_vPos = m_pCurrentWorkingObject->GetCurrentPosition();
            l_vPos.x += m_pCurrentWorkingObject->GetCurrentPointData()->Size.x/2.f;
            l_vPos.y += m_pCurrentWorkingObject->GetCurrentPointData()->Size.y/2.f;
            cGameApp::RenderFont(l_vPos.x,l_vPos.y,(wchar_t*)m_strDescription.c_str());
        }
    }
    
    void    cSelectOptionByCueToStartCurveWithTime::DebugRender()
    {
        this->m_pCurrentWorkingObject->RenderCollide();
    }
    
    void    cSelectOptionByCueToStartCurveWithTime::Update(float e_fElpaseTime)
    {
        this->m_pCurrentWorkingObject->Update(e_fElpaseTime);
		if(this->IsSatisfiedCondition())
		{
			this->CallExternalFunction(this);
		}
    }
    
    cMultiSelectOptionByMPDI::cMultiSelectOptionByMPDI(char*e_strFileName)
    {
        LoadData(e_strFileName);
    }
    
    cMultiSelectOptionByMPDI::~cMultiSelectOptionByMPDI()
    {
    
    }
    
    void	cMultiSelectOptionByMPDI::DebugRender()
    {
        int l_iCOunt = this->Count();
        for( int i=0;i<l_iCOunt;++i )
        {
            this->m_ObjectList[i]->DebugRender();
        }
    }
    
    void    cMultiSelectOptionByMPDI::LoadData(char*e_strFileName)
    {
        m_AnimationParser.Destroy();
        this->Destroy();
        bool    l_b = m_AnimationParser.Parse(e_strFileName);
        assert(l_b);
        cMPDIList*l_pMPDIList = dynamic_cast<cMPDIList*>(m_AnimationParser.GetObject(0));
        assert(l_pMPDIList->Count() == 3&&"this file must has Selected Horver Idle 3 data");
        //find out how many option
        int l_iNumOption = l_pMPDIList->GetObject(0)->Count();
        for( int i=0;i<l_iNumOption;++i )
        {
            cSelectOptionByCueToStartCurveWithTime*l_pSelectOptionByCueToStartCurveWithTime = new cSelectOptionByCueToStartCurveWithTime(l_pMPDIList,i);
            this->AddObjectNeglectExist(l_pSelectOptionByCueToStartCurveWithTime);
        }
    }

    cScrollingMSOGroup::cScrollingMSOGroup(float e_fDistanceToScrolling,eOrientation e_eOrientation,float e_fScrollingTime)
    {
        m_bOnScrolling = false;
        m_fDistanceToScrolling = e_fDistanceToScrolling;
        m_fRestDistanceToScrolling = e_fDistanceToScrolling;
        m_eOrientation = e_eOrientation;
        this->m_ScrollingTimeCounter.SetTargetTime(e_fScrollingTime);
    }
    void    cScrollingMSOGroup::Update(float e_fElpaseTime)
    {
        cClickMouseBehaviorVector<cClickMouseBehavior>::Update(e_fElpaseTime);
        if( m_bOnScrolling )
        {
            float   l_fMoveDistance = m_fDistanceToScrolling*e_fElpaseTime/m_ScrollingTimeCounter.fTargetTime;
            if(!m_ScrollingTimeCounter.bTragetTimrReached)
                m_fRestDistanceToScrolling -= (float)fabs(l_fMoveDistance);
            else
            {
                if( l_fMoveDistance>0.f )
                    l_fMoveDistance = m_fRestDistanceToScrolling;
                else
                    l_fMoveDistance = -m_fRestDistanceToScrolling;
                m_bOnScrolling = false;
                m_fRestDistanceToScrolling = 0.f;
            }
            m_ScrollingTimeCounter.Update(e_fElpaseTime);            
            int l_iCount = this->Count();
            Vector3 l_vPos(l_fMoveDistance,l_fMoveDistance,0.f);
            if( m_eOrientation == eO_HORIZONTAL )
                l_vPos.y = 0.f;
            else
                l_vPos.x = 0.f;
            for( int i=0;i<l_iCount;++i )
            {
                this->m_ObjectList[i]->SetRelativePosition(l_vPos);
            }
        }
    }
//end namespace FATMING_CORE
}